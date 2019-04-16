list(APPEND interproc_src
        message.cpp
        pipe.cpp
        worker_subprocess.cpp
        )
if (UNIX OR APPLE)
    list(APPEND interproc_src
            pipe_posix.cpp
            subprocess_posix.cpp
            )
endif ()

add_library(common_interproc_obj OBJECT ${interproc_src})
