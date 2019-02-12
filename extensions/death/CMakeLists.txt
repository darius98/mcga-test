add_library(kktest_extension_death
    src/check_death.cpp
    src/death_status.cpp
    src/matchers.cpp
)
target_include_directories(kktest_extension_death PUBLIC include)
target_link_libraries(kktest_extension_death PUBLIC kktest_interface kktest_extension_core_matchers PRIVATE common_interproc)

install(DIRECTORY include DESTINATION .)

add_kktest(kktest_extension_death_test
    tests/concepts.cpp
)
