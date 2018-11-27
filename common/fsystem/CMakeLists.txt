add_library(fsystem
    src/error.cpp
    src/extension.cpp
    src/file.cpp
    src/folder.cpp
    src/path.cpp
)
target_include_directories(fsystem PUBLIC include)
target_link_libraries(fsystem strutil)
