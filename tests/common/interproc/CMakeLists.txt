add_kktest(common_interproc_unittests
    $<TARGET_OBJECTS:common_interproc_obj>
    message_test.cpp
    pipe_test.cpp
    subprocess_test.cpp
    worker_subprocess_test.cpp
)
