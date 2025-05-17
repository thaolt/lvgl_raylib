cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME} LANGUAGES C)

set(BUILD_SHARED_LIBS OFF)

set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h)
set(LV_CONF_BUILD_DISABLE_EXAMPLES ON)
set(LV_CONF_BUILD_DISABLE_DEMOS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lvgl ${CMAKE_CURRENT_BINARY_DIR}/lvgl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} lvgl raylib lvgl_raylib)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lvgl)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../raylib)
