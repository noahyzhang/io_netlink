//
// Created by root on 22-10-8.
//

#include <sys/socket.h>
#include <linux/taskstats.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>
#include "io_stats.h"

int IoStatsCollect::init() {
    return netlink_.init();
}

int IoStatsCollect::update_all_thread_io_stats() {
    // 更新前先清空数组
    io_stats_arr_.clear();

    DIR *pr;
    if ((pr = opendir("/proc"))) {
        struct dirent *de = readdir(pr);

        for (; de; de = readdir(pr)) {
            char *eol = NULL;
            char path[30];
            int havt = 0;
            pid_t pid;
            DIR *tr;

            pid = strtol(de->d_name, &eol, 10);
            if (*eol != '\0')
                continue;
            snprintf(path, sizeof path, "/proc/%d/task", pid);
            if ((tr = opendir(path))) {
                struct dirent *tde = readdir(tr);

                for (; tde; tde = readdir(tr)) {
                    pid_t tid;

                    eol = NULL;
                    tid = strtol(tde->d_name, &eol, 10);
                    if (*eol != '\0')
                        continue;
                    havt = 1;
                    auto io_stats = std::make_shared<IoStats>();
                    auto res = get_thread_io_info(pid, tid, io_stats);
                    if (res < 0) {
                        std::cerr << "get_thread_io_info failed, pid: " << pid << ", tid: " << tid << ", errno: " << res
                                  << std::endl;
                    } else {
                        io_stats_arr_.emplace_back(io_stats);
                    }
                }
                closedir(tr);
            }
            // 主线程（tid == pid）
            if (!havt) {
                auto io_stats = std::make_shared<IoStats>();
                auto res = get_thread_io_info(pid, pid, io_stats);
                if (res < 0) {
                    std::cerr << "get_thread_io_info failed, pid: " << pid << ", tid: " << pid << ", errno: " << res
                              << std::endl;
                } else {
                    io_stats_arr_.emplace_back(io_stats);
                }
            }
        }
        closedir(pr);
    }
    return 0;
}

int IoStatsCollect::get_thread_io_info(pid_t pid, pid_t tid, const std::shared_ptr<IoStats> &io_stats) {
//    std::cout << "start gather pid: " << pid << ", tid: " << tid << " io info" << std::endl;
    if (netlink_.get_sock_fd() < 0) {
        std::cerr << "nl_sock: " << netlink_.get_sock_fd() << " is error" << std::endl;
        return -1;
    }
    // 注意 family_id 不能为0，否则会造成 recv 阻塞
    if (netlink_.get_netlink_family_id() <= 0) {
        std::cerr << "netlink_family_id: " << netlink_.get_netlink_family_id() << " is error" << std::endl;
        return -2;
    }
    // 获取 taskstats 中的数据
    auto res = netlink_.send_cmd(netlink_.get_netlink_family_id(), tid, TASKSTATS_CMD_GET,
                                 TASKSTATS_CMD_ATTR_PID, &tid, sizeof(tid));
    if (res < 0) {
        std::cerr << "netlink send_cmd failed, errno: " << res << std::endl;
        return -3;
    }
    io_stats->pid = pid;
    io_stats->tid = tid;

//    std::cout << "send cmd success, start recv" << std::endl;
    struct MsgTemplate msg;
    ssize_t rv = recv(netlink_.get_sock_fd(), &msg, sizeof(msg), 0);
    if (rv < 0 || !NLMSG_OK((&msg.nl_msg), (size_t) rv) || msg.nl_msg.nlmsg_type == NLMSG_ERROR) {
        struct nlmsgerr *err = reinterpret_cast<struct nlmsgerr *>(NLMSG_DATA(&msg));
        if (err->error != -ESRCH) {
            std::cerr << "recv failed, err: " << err->error << std::endl;
        }
        return -4;
    }
    rv = GENLMSG_PAYLOAD(&msg.nl_msg);
    struct nlattr *na = (struct nlattr *) GENLMSG_DATA(&msg);
    int len = 0;
    while (len < rv) {
        len += NLA_ALIGN(na->nla_len);
        if (na->nla_type == TASKSTATS_TYPE_AGGR_TGID || na->nla_type == TASKSTATS_TYPE_AGGR_PID) {
            int aggr_len = NLA_PAYLOAD(na->nla_len);
            int len2 = 0;

            na = (struct nlattr *) NLA_DATA(na);
            while (len2 < aggr_len) {
                if (na->nla_type == TASKSTATS_TYPE_STATS) {
                    struct taskstats *ts = reinterpret_cast<struct taskstats *>(NLA_DATA(na));
                    io_stats->read_bytes = ts->read_bytes;
                    io_stats->write_bytes = ts->write_bytes;
                    io_stats->swapin_delay_total = ts->swapin_delay_total;
                    io_stats->blkio_delay_total = ts->blkio_delay_total;
                }
                len2 += NLA_ALIGN(na->nla_len);
                na = reinterpret_cast<struct nlattr *>(reinterpret_cast<char *>(na) + len2);
            }
        }
        na = reinterpret_cast<struct nlattr *>(reinterpret_cast<char *>(GENLMSG_DATA(&msg)) + len);
    }
    return 0;
}

void IoStatsCollect::show_io_stats_arr() {
    // 展示方式
    // pid --- tid --- read_bytes --- write_bytes --- swapin_delay_total --- blkio_delay_total
    std::cout << std::setw(8) << "pid" << std::setw(8) << "tid"
              << std::setw(20) << "read_bytes" << std::setw(20) << "write_bytes"
              << std::setw(20) << "swapin_delay_total" << std::setw(20) << "blkio_delay_total"
              << std::endl;
    for (const auto &x: io_stats_arr_) {
        std::cout << std::setw(8) << x->pid << std::setw(8) << x->tid
                  << std::setw(20) << x->read_bytes << std::setw(20) << x->write_bytes
                  << std::setw(20) << x->swapin_delay_total << std::setw(20) << x->blkio_delay_total
                  << std::endl;
    }
}
