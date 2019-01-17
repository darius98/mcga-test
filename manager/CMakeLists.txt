add_executable(kktest_manager
    src/entry_point.cpp

    $<TARGET_OBJECTS:cppli_objects>
    $<TARGET_OBJECTS:kktest_string>
)
