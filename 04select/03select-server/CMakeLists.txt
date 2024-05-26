cmake_minimum_required(VERSION 3.2)
project(03select-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(03select-server main.cpp)
add_executable(client client.cpp)