
add_library(modules_lib
        string_builder.c
)

target_include_directories(modules_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
