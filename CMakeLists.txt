cmake_minimum_required(VERSION 3.10.2)
project(io_netlink)

set(CMAKE_CXX_STANDARD 11)

add_executable(io_netlink main.cpp netlink.cpp netlink.h io_stats.cpp io_stats.h)
