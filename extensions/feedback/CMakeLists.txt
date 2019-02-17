add_library(kktest_extension_feedback OBJECT
    src/ext.cpp
    src/test_logger.cpp
)
target_link_libraries(kktest_extension_feedback
    PUBLIC kktest_core
    PRIVATE termcolor
)
