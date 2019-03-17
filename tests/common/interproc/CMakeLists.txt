add_unit_test(common_interproc
    $<TARGET_OBJECTS:common_interproc_obj>
    message_test.cpp
    pipe_test.cpp
    subprocess_test.cpp
    worker_subprocess_test.cpp
)
