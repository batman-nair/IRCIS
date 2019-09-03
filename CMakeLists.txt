cmake_minimum_required(VERSION 3.0) 
project(ddrunner)


add_executable(ddrunner src/main.cc)

target_compile_options(ddrunner PUBLIC -Wall)

target_include_directories(ddrunner PUBLIC src)
