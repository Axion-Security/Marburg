cmake_minimum_required(VERSION 3.30)
project(Marburg)

set(CMAKE_CXX_STANDARD 14)

add_executable(Marburg main.cpp)

find_package(CURL REQUIRED)
target_link_libraries(Marburg PRIVATE CURL::libcurl)