add_library(kktest SHARED entry_point.cpp)
target_link_libraries(kktest kktest_interface
        cppli
        common_interproc
        common_string
        common_utils_time
        kktest_impl
        kktest_extension_core_matchers
        kktest_extension_death
        kktest_extension_feedback)
install(TARGETS kktest LIBRARY DESTINATION lib)
