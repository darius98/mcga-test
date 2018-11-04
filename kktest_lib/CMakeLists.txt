cmake_minimum_required(VERSION 3.7)
project(kktest VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

file(GLOB KKTestLibImpl
        src/*.cpp
        src/*.hpp
        src/*/*.cpp
        src/*/*.hpp
        src/*/*/*.cpp
        src/*/*/*.hpp
    )
add_library(kktest SHARED ${KKTestLibImpl})
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic -Werror)
target_include_directories(kktest PRIVATE src)
target_include_directories(kktest PRIVATE include)
target_link_libraries(kktest autojson easyflags)
set_target_properties(kktest PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1)
install(TARGETS kktest LIBRARY DESTINATION ${LIB_PATH})

if (NOT (BUILD_TYPE STREQUAL "DEV"))
    return()
endif ()

add_executable(concept_std_vector_test concept/std_vector_test.cpp)
install(TARGETS concept_std_vector_test DESTINATION concept_std_vector_test)
target_link_libraries(concept_std_vector_test kktest)

function(wiki_example FILE_NAME)
    set(T wiki_example_${FILE_NAME})
    add_executable(${T} wiki_examples/${FILE_NAME}.cpp)
    target_link_libraries(${T} kktest)
    install(TARGETS ${T} DESTINATION ${T})
endfunction()

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)

add_subdirectory(tests)
