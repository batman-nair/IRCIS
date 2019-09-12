cmake_minimum_required(VERSION 3.0)
project(ddrunner)

set(SRC_DIR src)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")

add_executable(ddrunner
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/Grid.cc
  ${SRC_DIR}/Logger.cc
  )

target_compile_options(ddrunner PRIVATE -Wall)

target_include_directories(ddrunner PRIVATE ${SRC_DIR})

# Have a copy of the exxecutable in the project root
add_custom_command(TARGET ddrunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ddrunner> ${PROJECT_SOURCE_DIR}
)
