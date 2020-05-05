cmake_minimum_required(VERSION 3.10)

#set project name
project( list_ut )

###############################################################################
## file globbing 
###############################################################################

file( GLOB_RECURSE sources src/*.c src/*.h main.c)
file( GLOB_RECURSE sources_test test/*.c )

###############################################################################
## target definitions
###############################################################################

#add executable
add_executable(list_ut ${sources})

# Just for example add some compiler flags.
target_compile_options(list_ut PUBLIC -std=c99 -Wall -Wfloat-conversion)
target_include_directories(list_ut PUBLIC src)