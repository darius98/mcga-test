add_custom_target(mcga_test_check COMMAND ${CMAKE_CTEST_COMMAND})
message(STATUS "${CMAKE_CTEST_COMMAND}")

include(FindThreads)

function(add_unit_test TEST_NAME)
    cmake_parse_arguments(P "NO_EXECUTOR_TYPE" "" "" ${ARGN})
    set(UNIT_TEST mcga_test_unit_${TEST_NAME})
    add_executable(${UNIT_TEST} ${P_UNPARSED_ARGUMENTS})
    target_link_libraries(${UNIT_TEST} mcga_test_static)
    add_dependencies(mcga_test_check ${UNIT_TEST})

    if (P_NO_EXECUTOR_TYPE)
        add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})
    else ()
        add_test(NAME ${UNIT_TEST}_boxed COMMAND ${UNIT_TEST} --executor=boxed)
        add_test(NAME ${UNIT_TEST}_smooth COMMAND ${UNIT_TEST} --executor=smooth)
    endif ()
endfunction()

function(add_integration_test TEST_NAME)
    set(INTEGRATION_TEST mcga_test_integration_${TEST_NAME})
    set(INTEGRATION_TEST_BIN ${INTEGRATION_TEST}_binary)
    add_executable(${INTEGRATION_TEST_BIN} ${ARGN})
    set_target_properties(${INTEGRATION_TEST_BIN} PROPERTIES
            RUNTIME_OUTPUT_NAME ${TEST_NAME}
            )
    target_link_libraries(${INTEGRATION_TEST_BIN} mcga_test_static ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(mcga_test_check ${INTEGRATION_TEST_BIN})
endfunction()

function(add_basic_test TEST_NAME)
    set(BASIC_TEST mcga_test_basic_${TEST_NAME})
    add_executable(${BASIC_TEST} ${ARGN})
    target_link_libraries(${BASIC_TEST} PUBLIC mcga_test_static)
    add_dependencies(mcga_test_check ${BASIC_TEST})

    add_test(NAME ${BASIC_TEST} COMMAND ${BASIC_TEST})
endfunction()

add_subdirectory(core)
add_subdirectory(extensions)
