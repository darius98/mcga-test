cmake_minimum_required(VERSION 3.8)
project(RuntimeTesting)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)
file(GLOB RuntimeTestingLibImpl src/**/*.cpp src/**/*.hpp runtime_testing)
add_library("runtime_testing" ${RuntimeTestingLibImpl})

set(T StdVectorTests)
add_executable(${T} concept/std_vector_test.cpp)
install(TARGETS ${T} DESTINATION ${T})
target_link_libraries(${T} "runtime_testing")

function(wiki_example TARGET_NAME FILE_NAME)
    set(T WikiExample${TARGET_NAME})
    add_executable(${T} wiki_examples/${FILE_NAME}.cpp)
    install(TARGETS ${T} DESTINATION ${T})
    target_link_libraries(${T} "runtime_testing")
endfunction()

wiki_example(Base base)
wiki_example(SetUpTearDownBad set_up_tear_down_bad)
wiki_example(SetUpTearDownGood set_up_tear_down_good)
