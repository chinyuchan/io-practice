cmake_minimum_required(VERSION 3.2)
project(05poll C)

set(CMAKE_C_STANDARD 11)

add_executable(05poll main.c)
add_executable(client client.c)