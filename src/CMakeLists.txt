
add_library(src_lib
        emblib_circ_buffer.c
        emblib_queue.c
        emblib_stack.c
        emblib_util.c
        emblib_deque.c
        emblib_list.c
        emblib_set.c
)

target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
