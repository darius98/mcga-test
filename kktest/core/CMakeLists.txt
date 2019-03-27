add_library(kktest_core OBJECT
    box_executor.cpp
    driver.cpp
    executed_test.cpp
    executor.cpp
    group.cpp
    kktest.cpp
    main.cpp
    time_tick.cpp
    test.cpp
    warning.cpp
)
target_compile_options(kktest_core PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest_core
    PUBLIC include
    PUBLIC ${cppli_include_dir}
)

install(DIRECTORY include DESTINATION .)
