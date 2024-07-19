
add_library(src_lib
    circ_buffer.c
    queue.c
    stack.c
    util.c
)

target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
