enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(
        main_test_util
        main_test_util.cpp
)

target_compile_options(main_test_util PRIVATE -std=gnu++17)

target_link_libraries(main_test_util PRIVATE gtest gtest_main src_lib)

enable_testing()

add_test(NAME main_test_util COMMAND main_test_util)
