set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cc
  Ircis.cc
  Runner.cc
  Grid.cc
  Logger.cc
  DirVec.cc
  RunnerStack.cc
  DataType.cc
  CharMaps.cc
  )
add_executable(${BINARY} ${SOURCES})

target_compile_options(${BINARY} PRIVATE -Wall -Wextra -Wpedantic -Werror)

include_directories(.)

add_library(${BINARY}_lib STATIC ${SOURCES})

# Have a copy of the executable in the project root
add_custom_command(TARGET ${BINARY} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ircis> ${PROJECT_SOURCE_DIR}/
  )
