#include <stdbool.h>
#include <stdint.h>
#include "lvgl_raylib.h"
#include "lvgl_raylib_display.h"
#include "lvgl_raylib_input.h"

/* private prototypes */

static uint32_t lvgl_raylib_tick_cb(void);

/* static variables */

static lvgl_raylib_display_t _default_display = {0};
static lvgl_raylib_input_t _default_input = {0};

/* PUBLIC IMPLEMENTATION */

void lvgl_raylib_init(int width, int height)
{
    lv_init();
    lv_tick_set_cb(&lvgl_raylib_tick_cb);
    lvgl_raylib_display_create(&_default_display, width, height);
    lvgl_raylib_input_create(&_default_input);
}

void lvgl_raylib_process_events(void)
{
    lv_indev_read(_default_input.keyboard_indev);
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
    lvgl_raylib_display_destroy(&_default_display);
    lvgl_raylib_input_destroy(&_default_input);
    lv_deinit();
}

/* PRIVATE IMPLEMENTATION */

static uint32_t lvgl_raylib_tick_cb(void) {
    return GetTime() * 1000;
}
