#include "raylib.h"
#include "lvgl.h"
#include "lvgl_raylib.h"

#define WIDTH 800
#define HEIGHT 600

void create_widgets() {
    /* create text area for name input */
    lv_obj_t * name_ta = lv_textarea_create(lv_screen_active());
    lv_textarea_set_placeholder_text(name_ta, "Enter your name");
    lv_obj_set_width(name_ta, 250);
    lv_obj_align(name_ta, LV_ALIGN_CENTER, 0, -50);
    /* create a simple button */
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_width(btn, 250);
    lv_obj_set_height(btn, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    /* create a label for the button */
    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click me");
    lv_obj_center(btn_label);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "LVGL Raylib Example");
    SetTargetFPS(60);

    lvgl_raylib_init(WIDTH, HEIGHT);
    create_widgets();

    while (!WindowShouldClose())
    {
        lvgl_raylib_process_events();

        BeginDrawing();
 
        ClearBackground(DARKGRAY);

        lvgl_raylib_render();

        EndDrawing();
    }

    lvgl_raylib_deinit();
    CloseWindow();
    return 0;
}