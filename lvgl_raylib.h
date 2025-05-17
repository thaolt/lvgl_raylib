#ifndef LVGL_RAYLIB_H
#define LVGL_RAYLIB_H

void lvgl_raylib_init(int width, int height);
void lvgl_raylib_process_events(void);
void lvgl_raylib_render(void);
void lvgl_raylib_deinit(void);

#endif