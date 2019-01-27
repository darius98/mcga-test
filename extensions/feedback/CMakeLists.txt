add_library(kktest_extension_feedback_test_logger OBJECT
    src/test_logger.cpp
)
target_include_directories(kktest_extension_feedback_test_logger PUBLIC include)
target_link_libraries(kktest_extension_feedback_test_logger PUBLIC kktest_interface)

add_library(kktest_extension_feedback OBJECT
    src/ext.cpp
)
target_link_libraries(kktest_extension_feedback kktest_extension_feedback_test_logger)

install(DIRECTORY extensions/feedback/include DESTINATION .)
