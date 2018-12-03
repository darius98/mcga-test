add_library(interproc
    src/message.cpp
    src/message_reader.cpp
    src/pipe.cpp
)
target_include_directories(interproc PUBLIC include)
