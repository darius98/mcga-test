set(interproc_src OBJECT
    src/message.cpp
    src/subprocess.cpp
)
if (UNIX)
    list(APPEND interproc_src src/unix/pipe.cpp src/unix/subprocess.cpp)
endif ()

add_library(interproc ${interproc_src})
target_link_libraries(interproc PUBLIC kktest_string)
target_include_directories(interproc PUBLIC include)
install(DIRECTORY include DESTINATION .)

add_kktest(interproc_test
    tests/message_test.cpp)