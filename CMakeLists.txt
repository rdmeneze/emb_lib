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

target_compile_options(main_test_queue PRIVATE -std=gnu++17)



###############################################################################
## file globbing
###############################################################################

#file( GLOB_RECURSE sources src/*.c src/*.h)
#file( GLOB_RECURSE sources_test test/*.c )

###############################################################################
## target definitions
###############################################################################

#add executable
#add_executable(main ${sources} ${sources_test})

# Just for example add some compiler flags.
#target_compile_options(main PUBLIC -Wall)
#set(CMAKE_C_STANDARD 11)
#target_include_directories(main PUBLIC src)
