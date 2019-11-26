set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES
  DataTypeTest.cc
  SystemTests.cc
  )

include(GoogleTest)

add_executable(${BINARY} ${SOURCES})

# add_test(NAME ${BINARY} COMMAND ${BINARY} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
gtest_discover_tests(${BINARY} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest_main)

# Have a copy of the executable in the project root
add_custom_command(TARGET ${BINARY} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BINARY}> ${PROJECT_SOURCE_DIR}/
  )
