add_library(arguments src/arguments.cpp)
target_include_directories(arguments PUBLIC include)
target_link_libraries(arguments PUBLIC strutil)
