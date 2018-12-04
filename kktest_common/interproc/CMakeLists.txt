set(interproc_src src/message.cpp src/message_reader.cpp)
if (UNIX)
    list(APPEND interproc_src src/unix/pipe.cpp src/unix/subprocess.cpp)
endif ()

add_library(interproc ${interproc_src})
target_include_directories(interproc PUBLIC include)
