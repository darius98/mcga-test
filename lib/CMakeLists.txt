cmake_minimum_required(VERSION 3.8.2)
project(kktest VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB KKTestLibImpl
        include/kktest
        include/kktest_matchers
        include/*.hpp
        include/*/*.hpp
        include/*/*/*.hpp
        src/*.cpp
        src/*.hpp
        src/*/*.cpp
        src/*/*.hpp
        src/*/*/*.cpp
        src/*/*/*.hpp
    )
add_library(kktest SHARED ${KKTestLibImpl})
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest PRIVATE src)
target_include_directories(kktest PRIVATE include)
target_include_directories(kktest PRIVATE include/kktest_public)
target_link_libraries(kktest messaging strutil easyflags)
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
target_include_directories(concept_std_vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

function(wiki_example FILE_NAME)
    set(T wiki_example_${FILE_NAME})
    add_executable(${T} wiki_examples/${FILE_NAME}.cpp)
    target_link_libraries(${T} kktest)
    target_include_directories(${T} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    install(TARGETS ${T} DESTINATION ${T})
endfunction()

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)

add_subdirectory(tests)
