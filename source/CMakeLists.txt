project(slazylist)
cmake_minimum_required(VERSION 3.2)

add_library(slazylist STATIC slazylist.c)
add_executable(main main.c)

target_link_libraries(main PRIVATE slazylist)