#ifndef LVGL_RAYLIB_DISPLAY_H
#define LVGL_RAYLIB_DISPLAY_H

#include "lvgl.h"
#include "raylib.h"
#include <stdbool.h>

/* public types */

typedef struct {
    lv_draw_buf_t * draw_buf1;
    lv_draw_buf_t * draw_buf2;
    lv_display_t * disp;
    Image raylib_img;
    Texture2D raylib_texture;
    bool texture_created;
    bool texture_updated;
} lvgl_raylib_display_t;

/* public functions */

void lvgl_raylib_display_create(lvgl_raylib_display_t * display, int width, int height);
void lvgl_raylib_display_destroy(lvgl_raylib_display_t * display);

#endif