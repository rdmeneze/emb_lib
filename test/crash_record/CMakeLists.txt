enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/modules)

add_executable(
        main_test_crash_record
        main_test_crash_record.cpp
)

target_compile_options(main_test_crash_record PRIVATE -std=gnu++17)

target_link_libraries(main_test_crash_record PRIVATE gtest gtest_main modules_lib)

include(GoogleTest)
gtest_discover_tests(main_test_crash_record)

enable_testing()

add_test(NAME main_test_crash_record COMMAND main_test_crash_record)
