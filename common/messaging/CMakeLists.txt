add_library(messaging
    src/message.cpp
    src/message_reader.cpp
    src/input_pipe.cpp
    src/output_pipe.cpp
)
target_include_directories(messaging PUBLIC include)
target_include_directories(messaging PRIVATE include/messaging_public)
