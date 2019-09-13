cmake_minimum_required(VERSION 3.0)
project(ptrain)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -Wall")


add_subdirectory(src)
