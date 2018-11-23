cmake_minimum_required(VERSION 3.7)
project(Messaging VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(messaging
    src/message.cpp
    src/message_reader.cpp
    src/input_pipe.cpp
    src/output_pipe.cpp
)
target_include_directories(messaging PUBLIC include)
target_include_directories(messaging PRIVATE include/messaging_public)
