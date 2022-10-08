//
// Created by root on 22-10-8.
//

#ifndef IO_NETLINK_IO_STATS_H
#define IO_NETLINK_IO_STATS_H

#include <pthread.h>
#include <cstdint>
#include <memory>
#include <vector>
#include "netlink.h"

// 存储 IO 状态的结构体
struct IoStats {
    pid_t pid;
    pid_t tid;
    uint64_t swapin_delay_total;  // ns
    uint64_t blkio_delay_total;  // ns
    uint64_t read_bytes;
    uint64_t write_bytes;
};

class IoStatsCollect {
public:
    IoStatsCollect() = default;

    int init();

    int update_all_thread_io_stats();

    void show_io_stats_arr();

private:
    int get_thread_io_info(pid_t pid, pid_t tid, const std::shared_ptr<IoStats> &io_stats);

private:
    Netlink netlink_;
    std::vector<std::shared_ptr<IoStats>> io_stats_arr_;
};


#endif //IO_NETLINK_IO_STATS_H
