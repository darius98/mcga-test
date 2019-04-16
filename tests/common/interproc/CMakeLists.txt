add_unit_test(common_interproc
        $<TARGET_OBJECTS:common_interproc_obj>
        unit_message.cpp
        unit_pipe.cpp
        unit_subprocess.cpp
        unit_worker_subprocess.cpp
        )
