add_subdirectory(core)
add_subdirectory(extensions/matchers)
add_subdirectory(extensions/death)
add_subdirectory(extensions/feedback)

add_library(kktest SHARED
        $<TARGET_OBJECTS:common_interproc_obj>

        entry_point.cpp
        $<TARGET_OBJECTS:kktest_core>
        $<TARGET_OBJECTS:kktest_extension_death>
        $<TARGET_OBJECTS:kktest_extension_feedback>
        )
target_link_libraries(kktest
        PUBLIC kktest_core
        PUBLIC kktest_extension_matchers
        PUBLIC kktest_extension_death
        PUBLIC kktest_extension_feedback)
install(TARGETS kktest DESTINATION lib)
