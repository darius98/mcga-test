add_library(kktest_core OBJECT
    src/box_executor.cpp
    src/driver.cpp
    src/errors.cpp
    src/executor.cpp
    src/extension_api.cpp
    src/group.cpp
    src/kktest.cpp
    src/main.cpp
    src/test.cpp
    src/test_case_registry.cpp
)
target_compile_options(kktest_core PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest_core PUBLIC include)
target_link_libraries(kktest_core
    PUBLIC common_interproc
    PUBLIC common_utils
    PUBLIC cppli
)

install(DIRECTORY include DESTINATION .)
