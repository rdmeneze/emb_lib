cmake_minimum_required(VERSION 3.10)

#set project name
project( list_ut )

###############################################################################
## file globbing 
###############################################################################

file( GLOB_RECURSE sources src/*.c src/*.h)
file( GLOB_RECURSE sources_test test/*.c )

###############################################################################
## target definitions
###############################################################################

#add executable
add_executable(main ${sources} ${sources_test})

# Just for example add some compiler flags.
target_compile_options(main PUBLIC -Wall)
set(CMAKE_C_STANDARD 11)
target_include_directories(main PUBLIC src)
