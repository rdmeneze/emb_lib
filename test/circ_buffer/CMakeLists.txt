enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(
        main_test_circ_buffer
        main_test_circ_buffer.cpp
)

target_compile_options(main_test_circ_buffer PRIVATE -std=gnu++17)

target_link_libraries(main_test_circ_buffer PRIVATE gtest gtest_main src_lib)

enable_testing()

add_test(NAME main_test_circ_buffer COMMAND main_test_circ_buffer)
