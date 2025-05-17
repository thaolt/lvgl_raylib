#ifndef LVGL_RAYLIB_INPUT_H
#define LVGL_RAYLIB_INPUT_H

#include "lvgl.h"

typedef struct {
    lv_indev_t *mouse_indev;
    lv_indev_t *keyboard_indev;
    lv_group_t *group;
} lvgl_raylib_input_t;

void lvgl_raylib_input_create(lvgl_raylib_input_t *input);

#endif