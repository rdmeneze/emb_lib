enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/modules)

# compiled as C with the strings stripped, to check that configuration
add_library(assert_no_strings_helper OBJECT no_strings_helper.c)
target_compile_definitions(assert_no_strings_helper PRIVATE EMBLIB_ASSERT_NO_STRINGS)

add_executable(
        main_test_assert
        main_test_assert.cpp
        $<TARGET_OBJECTS:assert_no_strings_helper>
)

target_compile_options(main_test_assert PRIVATE -std=gnu++17)

target_link_libraries(main_test_assert PRIVATE gtest gtest_main modules_lib)

include(GoogleTest)
gtest_discover_tests(main_test_assert)

enable_testing()

add_test(NAME main_test_assert COMMAND main_test_assert)
