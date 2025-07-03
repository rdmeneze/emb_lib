cmake_minimum_required(VERSION 3.28)

project(emb_lib C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# =============================================================================
# THREAD SAFETY OPTIONS
# =============================================================================

option(EMBLIB_THREAD_SAFETY "Enable thread safety support" OFF)
option(EMBLIB_ATOMICS "Use C11 atomics for thread safety" OFF)
option(EMBLIB_EXPERIMENTAL "Enable experimental thread safety implementations" OFF)

set(EMBLIB_THREAD_BACKEND "PTHREAD" CACHE STRING "Thread backend (PTHREAD/FREERTOS/WINDOWS/NONE)")
set_property(CACHE EMBLIB_THREAD_BACKEND PROPERTY STRINGS "PTHREAD;FREERTOS;WINDOWS;NONE")

# =============================================================================
# THREAD SAFETY CONFIGURATION
# =============================================================================

if(EMBLIB_THREAD_SAFETY OR EMBLIB_ATOMICS)
    message(STATUS "Thread safety enabled")

    if(EMBLIB_ATOMICS)
        # Check for C11 atomics support
        include(CheckCSourceCompiles)
        check_c_source_compiles("
            #include <stdatomic.h>
            int main() {
                atomic_int x = ATOMIC_VAR_INIT(0);
                atomic_store(&x, 1);
                return atomic_load(&x);
            }" HAVE_C11_ATOMICS)

        if(HAVE_C11_ATOMICS)
            add_compile_definitions(EMBLIB_USE_ATOMICS)
            message(STATUS "Using C11 atomics for thread safety")
        else()
            message(WARNING "C11 atomics not supported, falling back to mutex")
            set(EMBLIB_ATOMICS OFF)
        endif()
    endif()

    if(NOT EMBLIB_ATOMICS)
        # Configure threading backend
        if(EMBLIB_THREAD_BACKEND STREQUAL "PTHREAD")
            find_package(Threads REQUIRED)
            add_compile_definitions(EMBLIB_USE_PTHREAD)
            set(THREAD_LIBRARIES Threads::Threads)
            message(STATUS "Using pthread for thread safety")

        elseif(EMBLIB_THREAD_BACKEND STREQUAL "FREERTOS")
            add_compile_definitions(EMBLIB_USE_FREERTOS)
            message(STATUS "Using FreeRTOS for thread safety")

        elseif(EMBLIB_THREAD_BACKEND STREQUAL "WINDOWS")
            add_compile_definitions(EMBLIB_USE_WINDOWS)
            message(STATUS "Using Windows threads for thread safety")

        endif()
    endif()

    add_compile_definitions(EMBLIB_THREAD_SAFETY_ENABLED)
else()
    message(STATUS "Thread safety disabled")
endif()

# =============================================================================
# EXISTING CONFIGURATION
# =============================================================================

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# =============================================================================
# PROJECT STRUCTURE
# =============================================================================

add_subdirectory(src)
add_subdirectory(modules)
add_subdirectory(test)
add_subdirectory(test/queue)
add_subdirectory(test/util)
add_subdirectory(test/circ_buffer)
add_subdirectory(test/stack)
add_subdirectory(test/deque)
add_subdirectory(test/list)
add_subdirectory(test/set)
add_subdirectory(test/string_builder)

# Experimental thread safety implementations
if(EMBLIB_EXPERIMENTAL)
    message(STATUS "Including experimental thread safety implementations")
    # add_subdirectory(experiments)  # Uncomment when ready
endif()

message(STATUS "Including directory: ${CMAKE_SOURCE_DIR}/src")

# =============================================================================
# BUILD INFORMATION
# =============================================================================

message(STATUS "=== emb_lib Build Configuration ===")
message(STATUS "Thread Safety: ${EMBLIB_THREAD_SAFETY}")
message(STATUS "C11 Atomics: ${EMBLIB_ATOMICS}")
if(EMBLIB_THREAD_SAFETY AND NOT EMBLIB_ATOMICS)
    message(STATUS "Thread Backend: ${EMBLIB_THREAD_BACKEND}")
endif()
message(STATUS "Experimental: ${EMBLIB_EXPERIMENTAL}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "=====================================")

#target_compile_options(main_test_queue PRIVATE -std=gnu++17)
