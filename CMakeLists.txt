cmake_minimum_required(VERSION 3.28)

project( emb_lib C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(test/queue)
add_subdirectory(test/util)
add_subdirectory(test/circ_buffer)
add_subdirectory(test/stack)
add_subdirectory(test/deque)
add_subdirectory(test/list)

message(STATUS "Including directory: ${CMAKE_SOURCE_DIR}/src")


#target_compile_options(main_test_queue PRIVATE -std=gnu++17)
