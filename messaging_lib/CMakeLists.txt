cmake_minimum_required(VERSION 3.7)
project(messaginglib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(messaginglib
    include/message.hpp
    include/message_reader.hpp
    include/input_pipe.hpp
    include/output_pipe.hpp
    include/messaging
    src/message.cpp
    src/message_reader.cpp
    src/input_pipe.cpp
    src/output_pipe.cpp
)
target_include_directories(messaginglib PUBLIC include)
