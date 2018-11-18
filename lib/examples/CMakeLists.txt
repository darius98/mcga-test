cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)

add_executable(example_std_vector_test std_vector_test.cpp)
target_link_libraries(example_std_vector_test kktest)

function(wiki_example FILE_NAME)
    add_executable("wiki_example_${FILE_NAME}" wiki/${FILE_NAME}.cpp)
    target_link_libraries("wiki_example_${FILE_NAME}" kktest)
endfunction()

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)
