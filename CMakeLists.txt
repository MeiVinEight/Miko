cmake_minimum_required(VERSION 3.23)
project(Miko)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Memory)
add_subdirectory(Timestamp)
add_subdirectory(String)
add_subdirectory(Exception)
add_subdirectory(Filesystem)
add_subdirectory(WSA)
add_subdirectory(HTTP)
add_subdirectory(CWS)
add_subdirectory(WST)