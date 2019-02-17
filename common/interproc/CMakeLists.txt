list(APPEND interproc_src
    src/message.cpp
    src/pipe.cpp
    src/subprocess.cpp
)
if (LINUX OR APPLE)
    list(APPEND interproc_src src/posix/pipe.cpp src/posix/subprocess.cpp)
endif ()

add_library(common_interproc ${interproc_src})
target_link_libraries(common_interproc PUBLIC common_string)
target_include_directories(common_interproc PUBLIC include)

add_kktest(common_interproc_test
    tests/message_test.cpp
    tests/pipe_test.cpp)
