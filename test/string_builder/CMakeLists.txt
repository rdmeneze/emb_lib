enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/modules)

add_executable(
        main_test_string_buffer
        test_string_buffer.cpp
)

target_compile_options(main_test_string_buffer PRIVATE -std=gnu++17)

target_link_libraries(main_test_string_buffer PRIVATE gtest gtest_main modules_lib)

include(GoogleTest)
gtest_discover_tests(main_test_string_buffer)

enable_testing()

add_test(NAME main_test_string_buffer COMMAND main_test_string_buffer)
