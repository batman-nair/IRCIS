cmake_minimum_required(VERSION 3.0)
project(ircis)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)
