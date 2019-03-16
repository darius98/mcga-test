set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/tests")

function(add_kktest TEST_NAME)
    add_executable(${ARGV})
    target_link_libraries(${TEST_NAME} kktest)
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_subdirectory(common/interproc)
add_subdirectory(common/utils)
add_subdirectory(kktest/extensions/death)
add_subdirectory(kktest/extensions/matchers)
