
add_subdirectory(extensions/core_matchers)
add_subdirectory(extensions/feedback)
add_subdirectory(extensions/test_runner)

add_library(kktest
    generated/kktest_main.cpp

    $<TARGET_OBJECTS:cppli_objects>
    $<TARGET_OBJECTS:interproc>
    $<TARGET_OBJECTS:kktest_string>
    $<TARGET_OBJECTS:kktest_time>
    $<TARGET_OBJECTS:kktest_impl>

    $<TARGET_OBJECTS:kktest_extension_core_matchers>
    $<TARGET_OBJECTS:kktest_extension_feedback>
    $<TARGET_OBJECTS:kktest_extension_feedback_test_logger>

)

install(TARGETS kktest ARCHIVE DESTINATION lib)

target_link_libraries(kktest kktest_interface

    kktest_extension_core_matchers
    kktest_extension_feedback

)

install(TARGETS kktest_test_runner RUNTIME DESTINATION kktest_bin)

install(DIRECTORY extensions/feedback/include DESTINATION .)
install(DIRECTORY extensions/core_matchers/include DESTINATION .)
