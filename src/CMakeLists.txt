add_executable(ircis
  main.cc
  Ircis.cc
  Runner.cc
  Grid.cc
  Logger.cc
  DirVec.cc
  RunnerStack.cc
  DataType.cc
  CharMaps.cc
  )

target_compile_options(ircis PRIVATE -Wall -Wextra -Wpedantic -Werror)

include_directories(.)

# Have a copy of the executable in the project root
add_custom_command(TARGET ircis POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ircis> ${PROJECT_SOURCE_DIR}/
  )
