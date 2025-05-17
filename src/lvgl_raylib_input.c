#include "lvgl_raylib_input.h"
#include "raylib.h"
#include "lvgl.h"

static void lvgl_raylib_pointer_read(lv_indev_t * indev, lv_indev_data_t* data);
static void lvgl_raylib_keyboard_read(lv_indev_t * indev, lv_indev_data_t* data);


void lvgl_raylib_input_create(lvgl_raylib_input_t *input) {
    // Create and initialize the pointer input device (mouse or touch)
    input->mouse_indev = lv_indev_create();
    if (input->mouse_indev == NULL) {
        TraceLog(LOG_ERROR, "Failed to create LVGL pointer input device");
        return;
    }
    
    lv_indev_set_type(input->mouse_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(input->mouse_indev, lvgl_raylib_pointer_read);
    lv_indev_set_display(input->mouse_indev, lv_display_get_default());
    
    // Create and initialize the keypad input device (keyboard)
    input->keyboard_indev = lv_indev_create();
    if (input->keyboard_indev == NULL) {
        TraceLog(LOG_ERROR, "Failed to create LVGL keypad input device");
        return;
    }
    
    lv_indev_set_type(input->keyboard_indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_mode(input->keyboard_indev, LV_INDEV_MODE_EVENT);
    lv_indev_set_read_cb(input->keyboard_indev, lvgl_raylib_keyboard_read);
    lv_indev_set_display(input->keyboard_indev, lv_display_get_default());

    input->group = lv_group_create();
    lv_group_set_default(input->group);

    lv_indev_set_group(input->mouse_indev, input->group);
    lv_indev_set_group(input->keyboard_indev, input->group);
}

static void lvgl_raylib_pointer_read(lv_indev_t * indev, lv_indev_data_t* data) {
    // Get mouse position
    Vector2 mouse_pos = GetMousePosition();
    
    // Update coordinates
    data->point.x = (int16_t)mouse_pos.x;
    data->point.y = (int16_t)mouse_pos.y;
    
    // Update pointer state (pressed or released)
    data->state = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    
    // Get mouse wheel movement since last read
    int16_t wheel_diff = GetMouseWheelMove();
    
    // Update encoder difference for scrolling
    data->enc_diff = wheel_diff * 10;
    
    // We do not need to continue reading
    data->continue_reading = false;
}

// Convert Raylib key codes to LVGL key codes
static uint32_t convert_control_key(int key) {
    switch (key) {
        case KEY_UP:    return LV_KEY_UP;
        case KEY_DOWN:  return LV_KEY_DOWN;
        case KEY_RIGHT: return LV_KEY_RIGHT;
        case KEY_LEFT:  return LV_KEY_LEFT;
        case KEY_ENTER: return LV_KEY_ENTER;
        case KEY_ESCAPE: return LV_KEY_ESC;
        case KEY_BACKSPACE: return LV_KEY_BACKSPACE;
        case KEY_TAB:   return LV_KEY_NEXT;
        case KEY_HOME:  return LV_KEY_HOME;
        case KEY_END:   return LV_KEY_END;
        case KEY_DELETE: return LV_KEY_DEL;
        default:        return 0;
    }
}

static void lvgl_raylib_keyboard_read(lv_indev_t * indev, lv_indev_data_t* data) {
    // First check for special keys
    int key = GetKeyPressed();
    if (key) {
        uint32_t lvgl_key = convert_control_key(key);
        if (lvgl_key && lvgl_key != LV_KEY_ESC) { /* ignore ESC key */
            data->key = lvgl_key;
            data->state = LV_INDEV_STATE_PRESSED;
            data->continue_reading = true;
            return;
        }
    }
    
    // If no special key, check for character input
    int char_key = GetCharPressed();
    if (char_key) {
        data->key = char_key;
        data->state = LV_INDEV_STATE_PRESSED;
        data->continue_reading = true;
        return;
    }
    
    // If we get here, no key is pressed
    data->state = LV_INDEV_STATE_RELEASED;
    data->continue_reading = false;
}