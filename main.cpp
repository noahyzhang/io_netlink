
#include <iostream>
#include <chrono>
#include "io_stats.h"

std::string get_cur_system_time() {
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&t);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d-%02d.%02d.%02d", (int) ptm->tm_year + 1900,
            (int) ptm->tm_mon + 1, (int) ptm->tm_mday, (int) ptm->tm_hour, (int) ptm->tm_min, (int) ptm->tm_sec);
    return std::string(date);
}

int main() {
    IoStatsCollect io_stats_collect;
    auto res = io_stats_collect.init();
    if (res < 0) {
        std::cerr << "init failed, errno: " << res << std::endl;
        return -1;
    }
    std::cout << "time: " << get_cur_system_time() << std::endl;
    io_stats_collect.update_all_thread_io_stats();
    io_stats_collect.show_io_stats_arr();
    return 0;
}
