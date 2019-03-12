add_library(kktest_core OBJECT
    src/box_executor.cpp
    src/driver.cpp
    src/executed_test.cpp
    src/executor.cpp
    src/group.cpp
    src/kktest.cpp
    src/main.cpp
    src/time_tick_length.cpp
    src/test.cpp
)
target_compile_options(kktest_core PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest_core PUBLIC include)

install(DIRECTORY include DESTINATION .)
