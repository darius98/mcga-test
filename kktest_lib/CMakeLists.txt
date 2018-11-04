cmake_minimum_required(VERSION 3.7)
project(kktest VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

set(BUILD_TYPE "DEV" CACHE STRING "BUILD_TYPE")

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

set(KKTEST_DIR .)
set(KKTEST_SRC_DIR ./src)
set(KKTEST_THIRD_PARTY_DIR ../third_party)

# Third party software: autojson
set(AUTOJSON_DIR "${KKTEST_THIRD_PARTY_DIR}/autojson")
file(GLOB AUTOJSON_SRC ${AUTOJSON_DIR}/src/*.cpp)
add_library(autojson SHARED ${AUTOJSON_SRC})
target_compile_definitions(autojson PUBLIC autojsonuselib)
target_include_directories(autojson PUBLIC "${AUTOJSON_DIR}/include")

# Third party software: easyflags
set(EASYFLAGS_DIR "${KKTEST_THIRD_PARTY_DIR}/easyflags")
add_library(easyflags SHARED "${EASYFLAGS_DIR}/src/EasyFlags.cpp")
target_compile_definitions(easyflags PUBLIC easyflagsuselib)
target_include_directories(easyflags PUBLIC "${EASYFLAGS_DIR}/include")
target_link_libraries(easyflags autojson)

file(GLOB KKTestLibImpl
        ${KKTEST_SRC_DIR}/*.cpp
        ${KKTEST_SRC_DIR}/*.hpp
        ${KKTEST_SRC_DIR}/*/*.cpp
        ${KKTEST_SRC_DIR}/*/*.hpp
        ${KKTEST_SRC_DIR}/*/*/*.cpp
        ${KKTEST_SRC_DIR}/*/*/*.hpp
    )
add_library(kktest SHARED ${KKTestLibImpl})
target_include_directories(kktest PRIVATE ${KKTEST_SRC_DIR})
target_include_directories(kktest PRIVATE ${KKTEST_INCLUDE_DIR})
target_link_libraries(kktest autojson easyflags)
set_target_properties(kktest PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1)
install(TARGETS kktest LIBRARY DESTINATION ${LIB_PATH})

if (BUILD_TYPE STREQUAL "DEV")
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
endif()
