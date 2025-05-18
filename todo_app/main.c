#include "raylib.h"
#include "lvgl.h"
#include "lvgl_raylib.h"

#define WIDTH 800
#define HEIGHT 600
#define MAX_TASKS 20
#define TASK_MAX_LENGTH 100

typedef struct {
    char text[TASK_MAX_LENGTH];
    bool completed;
} Task;

// Global variables
static Task tasks[MAX_TASKS];
static int task_count = 0;
static lv_obj_t *task_list;
static lv_obj_t *task_input;
static lv_obj_t *add_button;

// Callback for toggling task completion
static void toggle_task_completion(lv_event_t* e) {
    lv_obj_t* label = lv_event_get_target(e);
    int* index_ptr = (int*)lv_obj_get_user_data(label);
    if (index_ptr) {
        // Toggle completion
        tasks[*index_ptr].completed = !tasks[*index_ptr].completed;
        update_task_list();
    }
}

// Callback for deleting a task
static void delete_task(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    int* index_ptr = (int*)lv_obj_get_user_data(btn);
    
    if (index_ptr) {
        // Delete task
        for (int j = *index_ptr; j < task_count - 1; j++) {
            tasks[j] = tasks[j + 1];
        }
        task_count--;
        free(index_ptr);
        update_task_list();
    }
}

void update_task_list() {
    // Clear existing list
    lv_obj_clean(task_list);

    // First, sort tasks with incomplete tasks first
    for (int i = 0; i < task_count - 1; i++) {
        for (int j = 0; j < task_count - i - 1; j++) {
            if (tasks[j].completed && !tasks[j+1].completed) {
                // Swap tasks
                Task temp = tasks[j];
                tasks[j] = tasks[j+1];
                tasks[j+1] = temp;
            }
        }
    }

    // Recreate list items
    for (int i = 0; i < task_count; i++) {
        // Create a container for each task
        lv_obj_t *task_container = lv_obj_create(task_list);
        lv_obj_set_width(task_container, lv_pct(100));
        lv_obj_set_layout(task_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(task_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(task_container, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        
        // Customize container style
        lv_obj_set_style_bg_color(task_container, lv_color_hex(0x1E1E1E), 0);
        lv_obj_set_style_border_width(task_container, 0, 0);
        lv_obj_set_style_pad_all(task_container, 5, 0);
        lv_obj_set_height(task_container, 40);
        
        // Disable scrollbar for this container
        lv_obj_remove_flag(task_container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_scrollbar_mode(task_container, LV_SCROLLBAR_MODE_OFF);

        // Create completion toggle button
        lv_obj_t *complete_btn = lv_btn_create(task_container);
        lv_obj_set_width(complete_btn, 40);
        lv_obj_set_height(complete_btn, 40);
        
        // Customize button style to be borderless and transparent
        lv_obj_set_style_bg_opa(complete_btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_opa(complete_btn, LV_OPA_10, LV_STATE_PRESSED);
        lv_obj_set_style_border_width(complete_btn, 0, 0);
        
        // Completion button label (icon)
        lv_obj_t *complete_label = lv_label_create(complete_btn);
        lv_label_set_text(complete_label, tasks[i].completed ? LV_SYMBOL_OK : LV_SYMBOL_MINUS);
        
        // Style the icon with hover effects
        lv_obj_set_style_text_color(complete_label, 
            tasks[i].completed ? lv_color_hex(0x00FF00) : lv_color_hex(0x888888), 0);
        lv_obj_set_style_text_color(complete_label, 
            tasks[i].completed ? lv_color_hex(0x00AA00) : lv_color_hex(0xBBBBBB), LV_STATE_PRESSED);
        
        // Center the icon
        lv_obj_center(complete_label);

        // Create task label
        lv_obj_t *task_label = lv_label_create(task_container);
        lv_label_set_text(task_label, tasks[i].text);
        lv_obj_set_flex_grow(task_label, 1);  // Allow task label to expand
        
        // Style for completed tasks
        if (tasks[i].completed) {
            lv_obj_set_style_text_decor(task_label, LV_TEXT_DECOR_STRIKETHROUGH, 0);
            lv_obj_set_style_text_color(task_label, lv_color_hex(0x888888), 0);
        } else {
            lv_obj_set_style_text_color(task_label, lv_color_hex(0xFFFFFF), 0);
        }

        // Create a delete button
        lv_obj_t *delete_btn = lv_btn_create(task_container);
        lv_obj_set_width(delete_btn, 40);
        lv_obj_set_height(delete_btn, 40);
        
        // Customize delete button style to be borderless and transparent
        lv_obj_set_style_bg_opa(delete_btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_opa(delete_btn, LV_OPA_10, LV_STATE_PRESSED);
        lv_obj_set_style_border_width(delete_btn, 0, 0);

        // Delete button label
        lv_obj_t *delete_label = lv_label_create(delete_btn);
        lv_label_set_text(delete_label, LV_SYMBOL_TRASH);
        
        // Style delete icon with hover effects
        lv_obj_set_style_text_color(delete_label, lv_color_hex(0xFF0000), 0);
        lv_obj_set_style_text_color(delete_label, lv_color_hex(0xAA0000), LV_STATE_PRESSED);

        // Center the trash icon
        lv_obj_center(delete_label);

        // Create a custom data structure to store the index
        int* task_index = malloc(sizeof(int));
        *task_index = i;

        // Store the task index as user data for label, complete button, and delete button
        lv_obj_set_user_data(task_label, task_index);
        lv_obj_set_user_data(complete_btn, task_index);
        lv_obj_set_user_data(delete_btn, task_index);

        // Add event callback for toggling completion
        lv_obj_add_event_cb(complete_btn, toggle_task_completion, LV_EVENT_CLICKED, NULL);

        // Add event callback for deleting task
        lv_obj_add_event_cb(delete_btn, delete_task, LV_EVENT_CLICKED, NULL);
    }
}

void add_task_event_cb(lv_event_t *e) {
    // Get text from input
    const char *task_text = lv_textarea_get_text(task_input);
    
    // Only add non-empty tasks
    if (strlen(task_text) > 0 && task_count < MAX_TASKS) {
        // Copy task text
        strncpy(tasks[task_count].text, task_text, TASK_MAX_LENGTH - 1);
        tasks[task_count].text[TASK_MAX_LENGTH - 1] = '\0';
        tasks[task_count].completed = false;
        
        // Increment task count
        task_count++;
        
        // Clear input
        lv_textarea_set_text(task_input, "");
        
        // Update list
        update_task_list();
    }
}

void create_ui_widgets() {
    // Set a dark background for the entire screen
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x1E1E1E), 0);

    // Create main container
    lv_obj_t *main_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_container, WIDTH - 40, HEIGHT - 40);
    lv_obj_center(main_container);
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(main_container, 20, 0);
    
    // Customize main container style
    lv_obj_set_style_bg_color(main_container, lv_color_hex(0x1E1E1E), 0);
    lv_obj_set_style_border_width(main_container, 0, 0);

    // Title
    lv_obj_t *title = lv_label_create(main_container);
    lv_label_set_text(title, "TODO List");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(title, lv_pct(100));
    lv_obj_set_style_text_font(title, LV_FONT_DEFAULT, 0);

    // Create a container for input and button
    lv_obj_t *input_container = lv_obj_create(main_container);
    lv_obj_set_width(input_container, lv_pct(100));
    lv_obj_set_layout(input_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(input_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(input_container, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(input_container, lv_color_hex(0x1E1E1E), 0);
    lv_obj_set_style_border_width(input_container, 0, 0);
    lv_obj_set_style_pad_column(input_container, 10, 0);

    // Task input area
    task_input = lv_textarea_create(input_container);
    lv_obj_set_flex_grow(task_input, 1);
    lv_textarea_set_placeholder_text(task_input, "Enter a new task...");
    lv_textarea_set_one_line(task_input, true);
    
    // Customize input style
    lv_obj_set_style_bg_color(task_input, lv_color_hex(0x2C2C2C), 0);
    lv_obj_set_style_text_color(task_input, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_color(task_input, lv_color_hex(0x3C3C3C), 0);

    // Add task button
    add_button = lv_btn_create(input_container);
    lv_obj_add_event_cb(add_button, add_task_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Customize button style
    lv_obj_set_style_bg_color(add_button, lv_color_hex(0x007BFF), 0);
    lv_obj_set_style_bg_color(add_button, lv_color_hex(0x0056b3), LV_STATE_PRESSED);
    
    lv_obj_t *add_label = lv_label_create(add_button);
    lv_label_set_text(add_label, "Add Task");
    lv_obj_set_style_text_color(add_label, lv_color_hex(0xFFFFFF), 0);

    // Task list
    task_list = lv_list_create(main_container);
    lv_obj_set_width(task_list, lv_pct(100));
    lv_obj_set_height(task_list, HEIGHT - 250);  // Adjust height as needed
    
    // Customize list style
    lv_obj_set_style_bg_color(task_list, lv_color_hex(0x1E1E1E), 0);
    lv_obj_set_style_border_width(task_list, 0, 0);

    // Add event callback for Enter key in task input
    lv_obj_add_event_cb(task_input, add_task_event_cb, LV_EVENT_READY, NULL);

    // Add some initial demo tasks
    strcpy(tasks[task_count].text, "Buy milk");
    tasks[task_count].completed = false;
    task_count++;

    strcpy(tasks[task_count].text, "Buy groceries");
    tasks[task_count].completed = false;
    task_count++;

    strcpy(tasks[task_count].text, "Call mom");
    tasks[task_count].completed = true;
    task_count++;

    // Update the task list with initial tasks
    update_task_list();
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "TODO App - LVGL/Raylib");
    SetTargetFPS(60);

    lvgl_raylib_init(WIDTH, HEIGHT);
    create_ui_widgets();

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