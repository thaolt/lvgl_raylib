cmake_minimum_required(VERSION 3.20)

project(lvgl_raylib LANGUAGES C)

add_subdirectory(lvgl_raylib_example)

add_library(lvgl_raylib STATIC
    src/lvgl_raylib.c
    src/lvgl_raylib_display.c
    src/lvgl_raylib_input.c
)

target_link_libraries(lvgl_raylib PRIVATE raylib lvgl)

target_include_directories(lvgl_raylib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})