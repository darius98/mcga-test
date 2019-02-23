list(APPEND interproc_src
    src/errors.cpp
    src/message.cpp
    src/pipe.cpp
    src/worker_subprocess.cpp
)
if (UNIX OR APPLE)
    list(APPEND interproc_src
        src/pipe_posix.cpp
        src/subprocess_posix.cpp
    )
endif ()

add_library(common_interproc ${interproc_src})
target_link_libraries(common_interproc PUBLIC common_utils)

add_kktest(common_interproc_test
    tests/main.cpp
    tests/message_test.cpp
    tests/pipe_test.cpp
    tests/subprocess_test.cpp
    tests/worker_subprocess_test.cpp
)
