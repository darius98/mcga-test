cmake_minimum_required(VERSION 3.7)
project(messaginglib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(messaginglib
    include/messaging
    include/messaging_public/message.hpp
    include/messaging_public/message_reader.hpp
    include/messaging_public/input_pipe.hpp
    include/messaging_public/output_pipe.hpp
    src/message.cpp
    src/message_reader.cpp
    src/input_pipe.cpp
    src/output_pipe.cpp
)
target_include_directories(messaginglib PUBLIC include)
target_include_directories(messaginglib PRIVATE include/messaging_public)
