cmake_minimum_required(VERSION 3.0)
project(ddrunner)

set(SRC_DIR src)

add_executable(ddrunner
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/Grid.cc)

target_compile_options(ddrunner PUBLIC -Wall)

target_include_directories(ddrunner PUBLIC ${SRC_DIR})

# Have a copy of the exxecutable in the project root
add_custom_command(TARGET ddrunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ddrunner> ${PROJECT_SOURCE_DIR}
)
