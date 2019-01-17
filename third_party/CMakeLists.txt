add_library(cppli_objects OBJECT
    cppli/src/cppli_impl.cpp
    cppli/src/cppli_interface.cpp
    cppli/src/argument_impl.cpp)
target_include_directories(cppli_objects PUBLIC cppli/include)

