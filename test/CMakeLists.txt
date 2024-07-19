enable_language(CXX)

include_directories(../src)


add_executable(
        main_test_queue
        main_test_queue.cpp
)

target_link_libraries(main_test_queue PRIVATE gtest gtest_main src_lib)

enable_testing()
add_test(NAME main_test_queue COMMAND main_test_queue)
