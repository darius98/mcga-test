cmake_minimum_required(VERSION 3.7)
project(messaginglib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(messaginglib
    include/message.hpp
    include/pipe.hpp
    src/message.cpp
    src/pipe.cpp
)
target_include_directories(messaginglib PUBLIC include)
