//
// Created by root on 22-10-8.
//

#ifndef IO_NETLINK_NETLINK_H
#define IO_NETLINK_NETLINK_H

#include <unistd.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <cstdint>

#define GENLMSG_DATA(glh)  ((void*)((char*)NLMSG_DATA(glh) + GENL_HDRLEN))
#define GENLMSG_PAYLOAD(glh)  (NLMSG_PAYLOAD(glh, 0) - GENL_HDRLEN)
#define NLA_DATA(na)  ((void*)((char*)(na) + NLA_HDRLEN))
#define NLA_PAYLOAD(len)  (len - NLA_HDRLEN)

#define MAX_MSG_SIZE 1024

// 发送消息的模版
struct MsgTemplate {
    struct nlmsghdr nl_msg;
    struct genlmsghdr genl_msg;
    char buf[MAX_MSG_SIZE];
};

// 封装 netlink socket 的相关操作
class Netlink {
public:
    Netlink() = default;

    ~Netlink() {
        if (sock_fd_ > -1) {
            close(sock_fd_);
        }
    }

    Netlink(const Netlink &) = delete;

    Netlink &operator=(const Netlink &) = delete;

    Netlink(Netlink &&) = delete;

    Netlink &operator=(Netlink &&) = delete;

    // 使用此类，先调用 init 方法进行初始化
    int init();

    int send_cmd(uint16_t nl_msg_type, uint32_t nl_msg_pid,
                 uint8_t genl_cmd, uint16_t nla_type, void *nla_data, int nla_len);

    inline int get_sock_fd() const {
        return sock_fd_;
    }

    inline int get_netlink_family_id() const {
        return netlink_family_id_;
    }

private:
    int init_sock();

    int init_netlink_family_id();

private:
    int sock_fd_ = -1;
    int netlink_family_id_ = -1;
};

#endif //IO_NETLINK_NETLINK_H
