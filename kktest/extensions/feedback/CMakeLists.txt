add_library(kktest_extension_feedback OBJECT
        ext.cpp
        test_logger.cpp
        )
target_link_libraries(kktest_extension_feedback
        PUBLIC kktest_core
        PRIVATE termcolor
        )
