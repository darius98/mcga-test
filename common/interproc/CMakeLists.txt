list(APPEND interproc_src
    src/message.cpp
    src/pipe.cpp
    src/subprocess.cpp
)
if (LINUX OR APPLE)
    list(APPEND interproc_src
        src/pipe_posix.cpp
        src/subprocess_posix.cpp
    )
endif ()

add_library(common_interproc ${interproc_src})
target_link_libraries(common_interproc PUBLIC common_string)

add_kktest(common_interproc_test
    tests/message_test.cpp
    tests/pipe_test.cpp)
