enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(
        main_test_queue
        main_test_queue.cpp
)

target_compile_options(main_test_queue PRIVATE -std=gnu++17)

target_link_libraries(main_test_queue PRIVATE gtest gtest_main src_lib)

include(GoogleTest)
gtest_discover_tests(main_test_queue)

enable_testing()

add_test(NAME main_test_queue COMMAND main_test_queue)
