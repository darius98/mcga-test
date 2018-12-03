add_library(interproc
    src/message.cpp
    src/message_reader.cpp
    src/pipe.cpp
    src/subprocess.cpp
)
target_include_directories(interproc PUBLIC include)
