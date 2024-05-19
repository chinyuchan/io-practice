cmake_minimum_required(VERSION 3.2)
project(03nonblock_multithread C)

set(CMAKE_C_STANDARD 11)

add_executable(server main.c)
add_executable(client client.c)