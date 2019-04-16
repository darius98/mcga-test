add_custom_target(mcga_test_check COMMAND ${CMAKE_CTEST_COMMAND})

function(add_unit_test TEST_NAME)
    set(UNIT_TEST mcga_test_unit_${TEST_NAME})
    add_executable(${UNIT_TEST} ${ARGN})
    target_link_libraries(${UNIT_TEST} kktest)
    add_dependencies(mcga_test_check ${UNIT_TEST})

    add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})
endfunction()

function(add_integration_test TEST_NAME SCRIPT_FILE)
    set(INTEGRATION_TEST mcga_test_integration_${TEST_NAME})
    set(INTEGRATION_TEST_BIN ${INTEGRATION_TEST}_binary)
    add_executable(${INTEGRATION_TEST_BIN} ${ARGN})
    set_target_properties(${INTEGRATION_TEST_BIN} PROPERTIES
            RUNTIME_OUTPUT_NAME ${TEST_NAME}
            )
    target_link_libraries(${INTEGRATION_TEST_BIN} kktest)
    add_dependencies(mcga_test_check ${INTEGRATION_TEST_BIN})

    add_test(NAME ${INTEGRATION_TEST}
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            )
endfunction()

function(add_basic_test TEST_NAME)
    set(BASIC_TEST mcga_test_basic_${TEST_NAME})
    add_executable(${BASIC_TEST} ${ARGN})
    add_dependencies(mcga_test_check ${BASIC_TEST})

    add_test(NAME ${BASIC_TEST} COMMAND ${BASIC_TEST})
endfunction()

add_subdirectory(kktest/extensions/death)
add_subdirectory(kktest/extensions/matchers)
add_subdirectory(kktest/core)
