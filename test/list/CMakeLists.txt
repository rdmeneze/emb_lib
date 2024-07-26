enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(
        main_test_list
        main_test_list.cpp
)

target_compile_options(main_test_list PRIVATE -std=gnu++17)

target_link_libraries(main_test_list PRIVATE gtest gtest_main src_lib)

include(GoogleTest)
gtest_discover_tests(main_test_list)

enable_testing()

add_test(NAME main_test_list COMMAND main_test_list)
