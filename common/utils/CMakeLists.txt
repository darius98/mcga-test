list(APPEND common_utils_src
    src/process_time.cpp
)
if (UNIX OR APPLE)
    list(APPEND common_utils_src
        src/process_time_posix.cpp
    )
endif ()

add_library(common_utils ${common_utils_src})

add_kktest(common_utils_test
    tests/process_time_test.cpp
)
