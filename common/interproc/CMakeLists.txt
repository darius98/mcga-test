list(APPEND interproc_src
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

add_library(common_interproc_obj OBJECT ${interproc_src})
target_link_libraries(common_interproc_obj PUBLIC common_utils)

add_kktest(common_interproc_test
    $<TARGET_OBJECTS:common_utils_obj>
    $<TARGET_OBJECTS:common_interproc_obj>
    tests/main.cpp
    tests/message_test.cpp
    tests/pipe_test.cpp
    tests/subprocess_test.cpp
    tests/worker_subprocess_test.cpp
)
