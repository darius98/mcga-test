list(APPEND common_utils_src
    src/time.cpp
)
if (UNIX OR APPLE)
    list(APPEND common_utils_src
        src/time_posix.cpp
    )
endif ()

add_library(common_utils ${common_utils_src})

add_kktest(common_utils_test
    tests/main.cpp
    tests/time_test.cpp
)
