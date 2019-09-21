add_executable(ptrain
  main.cc
  PTrain.cc
  Runner.cc
  Grid.cc
  Logger.cc
  DirVec.cc
  DataType.cc
  )

target_compile_options(ptrain PRIVATE -Wall -Wextra -Wpedantic -Werror)

include_directories(.)

# Have a copy of the executable in the project root
add_custom_command(TARGET ptrain POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ptrain> ${PROJECT_SOURCE_DIR}/
  )
