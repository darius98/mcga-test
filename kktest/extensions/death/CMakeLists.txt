add_library(kktest_extension_death OBJECT
        check_death.cpp
)
target_include_directories(kktest_extension_death PUBLIC include)
target_link_libraries(kktest_extension_death
    PUBLIC kktest_core
    PUBLIC kktest_extension_matchers
    PRIVATE common_interproc
)

install(DIRECTORY include DESTINATION .)
