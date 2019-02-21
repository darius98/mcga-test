add_library(kktest_extension_death OBJECT
    src/check_death.cpp
    src/death_status.cpp
    src/matchers.cpp
)
target_include_directories(kktest_extension_death PUBLIC include)
target_link_libraries(kktest_extension_death
    PUBLIC kktest_core
    PUBLIC kktest_extension_matchers
    PRIVATE common_interproc
)

install(DIRECTORY include DESTINATION .)

add_kktest(kktest_extension_death_test
    tests/main.cpp
    tests/check_death_test.cpp
    tests/matchers_test.cpp
)
