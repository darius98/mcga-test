add_executable(kktest_manager
    src/entry_point.cpp

    $<TARGET_OBJECTS:kktest_string>
)
target_link_libraries(kktest_manager PUBLIC cppli)
