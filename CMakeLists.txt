cmake_minimum_required(VERSION 3.8)
project(KTest)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)
file(GLOB KTestLibImpl src/**/*.cpp src/**/*.hpp ktest)
add_library("ktest" ${KTestLibImpl})

set(T StdVectorTests)
add_executable(${T} concept/std_vector_test.cpp)
install(TARGETS ${T} DESTINATION ${T})
target_link_libraries(${T} "ktest")

function(wiki_example FILE_NAME)
    set(T wiki_example_${FILE_NAME})
    add_executable(${T} wiki_examples/${FILE_NAME}.cpp)
    install(TARGETS ${T} DESTINATION ${T})
    target_link_libraries(${T} "ktest")
endfunction()

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)
