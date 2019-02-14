add_library(kktest SHARED entry_point.cpp)
target_link_libraries(kktest
    PUBLIC kktest_core
    PUBLIC kktest_extension_core_matchers
    PUBLIC kktest_extension_death
    PUBLIC kktest_extension_feedback)
install(TARGETS kktest LIBRARY DESTINATION lib)
