add_library(kktest_extension_feedback OBJECT
    src/ext.cpp
    src/test_logger.cpp
)
target_include_directories(kktest_extension_feedback PUBLIC include)
target_link_libraries(kktest_extension_feedback
    PUBLIC kktest_core
    PRIVATE termcolor
)
