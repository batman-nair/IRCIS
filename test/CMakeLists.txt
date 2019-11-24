set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES main.cc
  DataTypeTest.cc
  SystemTests.cc
  )

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

# Have a copy of the executable in the project root
add_custom_command(TARGET ${BINARY} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BINARY}> ${PROJECT_SOURCE_DIR}/
  )
