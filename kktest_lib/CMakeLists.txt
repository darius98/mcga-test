cmake_minimum_required(VERSION 3.7)
project(KKTestLib)
set(CMAKE_CXX_STANDARD 14)

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_build/lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

set(KKTEST_DIR .)
set(KKTEST_SRC_DIR ./src)
set(KKTEST_THIRD_PARTY_DIR ../third_party)

if(DEV)
    include("./kktest_library.cmake")
else()
    include("/usr/local/include/kktest.cmake")
endif()

add_executable(concept_std_vector_test concept/std_vector_test.cpp)
install(TARGETS concept_std_vector_test DESTINATION concept_std_vector_test)
target_link_libraries(concept_std_vector_test kktest kktest_matchers)

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
