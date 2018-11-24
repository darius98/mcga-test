cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)

add_executable(example_std_vector_test std_vector_test.cpp)
target_link_libraries(example_std_vector_test kktest)

function(example_wiki FILE_NAME)
    add_executable("example_wiki_${FILE_NAME}" wiki/${FILE_NAME}.cpp)
    target_link_libraries("example_wiki_${FILE_NAME}" kktest)
endfunction()

example_wiki(base)
example_wiki(set_up_tear_down)
example_wiki(group)
