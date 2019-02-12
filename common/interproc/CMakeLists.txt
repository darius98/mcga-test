list(APPEND interproc_src
    src/message.cpp
    src/subprocess.cpp
)
if (UNIX)
    list(APPEND interproc_src src/unix/pipe.cpp src/unix/subprocess.cpp)
endif ()

add_library(common_interproc ${interproc_src})
target_link_libraries(common_interproc PUBLIC common_string)
target_include_directories(common_interproc PUBLIC include)
install(DIRECTORY include DESTINATION .)

add_kktest(interproc_test
    tests/message_test.cpp
    tests/pipe_test.cpp)
