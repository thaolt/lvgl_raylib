#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lvgl_raylib.h"
#include "lvgl_raylib_display.h"

static void lvgl_raylib_display_flush_cb(lv_display_t * disp, const lv_area_t* area, uint8_t* px_map);

void lvgl_raylib_display_create(lvgl_raylib_display_t * display, int width, int height) {
    // create the display
    display->disp = lv_display_create(width, height);
    if (display->disp == NULL) {
        TraceLog(LOG_ERROR, "Failed to crlvgl_raylib_display_flush_cbeate LVGL display");
        return;
    }

    // Allocate memory for the draw buffers
    display->draw_buf1 = lv_draw_buf_create(width, height, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    if (display->draw_buf1 == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate LVGL draw buffer 1");
        return;
    }
    
    display->draw_buf2 = lv_draw_buf_create(width, height, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    if (display->draw_buf2 == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate LVGL draw buffer 2");
        lv_draw_buf_destroy(display->draw_buf1);
        display->draw_buf1 = NULL;
        return;
    }

    lv_display_set_driver_data(display->disp, display);
    lv_display_set_draw_buffers(display->disp, display->draw_buf1, display->draw_buf2);
    lv_display_set_color_format(display->disp, LV_COLOR_FORMAT_ARGB8888);
    lv_display_set_flush_cb(display->disp, &lvgl_raylib_display_flush_cb);
    lv_display_set_render_mode(display->disp, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_antialiasing(display->disp, true);
    lv_display_set_default(display->disp);
    
    // Initialize Raylib image and texture for hardware-accelerated rendering
    display->raylib_img.width = width;
    display->raylib_img.height = height;
    display->raylib_img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    display->raylib_img.mipmaps = 1;
    
    // Allocate memory for the image data (RGBA format)
    display->raylib_img.data = malloc(width * height * 4);
    if (display->raylib_img.data == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate Raylib image data");
        return;
    }
    
    // Clear image data to transparent
    memset(display->raylib_img.data, 0, width * height * 4);
    
    // Create the texture initially
    display->raylib_texture = LoadTextureFromImage(display->raylib_img);
    display->texture_created = true;
}

static void lvgl_raylib_display_flush_cb(lv_display_t * disp, const lv_area_t* area, uint8_t* px_map) {
    // find the display
    lvgl_raylib_display_t * display = (lvgl_raylib_display_t *)lv_display_get_driver_data(disp);
    if (display == NULL) {
        TraceLog(LOG_ERROR, "Failed to find LVGL Raylib display");
        return;
    }
    
    uint32_t area_width = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t area_height = (uint32_t)(area->y2 - area->y1 + 1);
    
    int32_t x_start = area->x1;
    int32_t y_start = area->y1;

    int32_t display_total_width = lv_display_get_horizontal_resolution(disp);
    unsigned char* img_data_buffer = (unsigned char*)display->raylib_img.data;

    // px_map is from LVGL, format is LV_COLOR_FORMAT_ARGB8888.
    // On little-endian systems, this means memory layout is B, G, R, A for each pixel.
    // When read as a uint32_t, it's (A << 24) | (R << 16) | (G << 8) | B.
    // Raylib's raylib_img.data is PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    // meaning memory layout R, G, B, A for each pixel.
    // When writing as a uint32_t, it should be (A << 24) | (B << 16) | (G << 8) | R.

    for (uint32_t y = 0; y < area_height; y++) {
        uint32_t* src_row_ptr_32 = (uint32_t*)(px_map + (y * area_width * 4));
        uint32_t* dst_row_ptr_32 = (uint32_t*)(img_data_buffer + ((y_start + y) * display_total_width + x_start) * 4);

        for (uint32_t x = 0; x < area_width; x++) {
            uint32_t src_pixel_value = src_row_ptr_32[x]; 
            // src_pixel_value on little-endian is (A_src << 24) | (R_src << 16) | (G_src << 8) | B_src

            uint8_t b_comp = (src_pixel_value & 0x000000FF);
            uint8_t g_comp = (src_pixel_value & 0x0000FF00) >> 8;
            uint8_t r_comp = (src_pixel_value & 0x00FF0000) >> 16;
            uint8_t a_comp = (src_pixel_value & 0xFF000000) >> 24;

            // Destination format RGBA in memory.
            // For little-endian uint32_t write, this is (A_dst << 24) | (B_dst << 16) | (G_dst << 8) | R_dst
            // We want R_dst = r_comp, G_dst = g_comp, B_dst = b_comp, A_dst = a_comp
            dst_row_ptr_32[x] = (a_comp << 24) | (b_comp << 16) | (g_comp << 8) | r_comp;
        }
    }

    display->texture_updated = true;
    lv_display_flush_ready(disp);
}