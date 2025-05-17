#include <stdbool.h>
#include <stdint.h>
#include "lvgl_raylib.h"
#include "lvgl_raylib_display.h"

/* private prototypes */

static uint32_t lvgl_raylib_tick_cb(void);
void lvgl_raylib_display_render_default(void);
void lvgl_raylib_input_create_default(void);
void lvgl_raylib_input_process_default(void);

/* static variables */

static lvgl_raylib_display_t _default_display = {0};

/* PUBLIC IMPLEMENTATION */

void lvgl_raylib_init(int width, int height)
{
    lv_init();
    lv_tick_set_cb(&lvgl_raylib_tick_cb);
    lvgl_raylib_display_create(&_default_display, width, height);
}

void lvgl_raylib_process_events(void)
{
    lv_task_handler();
}

void lvgl_raylib_render(void)
{
    // Only update the texture if LVGL has drawn something new
    if (_default_display.texture_updated && _default_display.texture_created) {
        UpdateTexture(_default_display.raylib_texture, _default_display.raylib_img.data);
        _default_display.texture_updated = false;
    }
    
    // Draw the texture on screen if it exists
    if (_default_display.texture_created) {
        DrawTexture(_default_display.raylib_texture, 0, 0, WHITE);
    }
}

void lvgl_raylib_deinit()
{
    lv_deinit();
}

/* PRIVATE IMPLEMENTATION */

static uint32_t lvgl_raylib_tick_cb(void) {
    return GetTime() * 1000;
}
