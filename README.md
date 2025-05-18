# lvgl_raylib

## Project Overview

`lvgl_raylib` is lightweight binding library designed to simplify desktop application development with minimal computational overhead. This project delivers efficient and responsive graphical interfaces through a strategic combination of two powerful libraries `LVGL` and `Raylib`.

## The reasons for the project's existence

In today's software landscape, complexity has become a silent burden. Electron-based applications epitomize this trend, where a simple todo list can inexplicably consume over 200MB of disk space—an inefficiency bordering on the absurd. In contrast, this project's example demonstrates a lean alternative, occupying just over 1MB when compiled with MinSizeRel and stripped.

Raylib offers an elegant and powerful API, but it lacks a comprehensive UI widget library. While raygui exists, it appears to have been largely abandoned. This gap presented an opportunity for a more robust solution.

Existing LVGL bindings for GLFW/OpenGLES/X11 were explored but found wanting. During testing, even a simple example would consistently consume 100% of a CPU core while remaining essentially idle—a performance characteristic that raised significant concerns. In stark contrast, combining LVGL with Raylib demonstrated remarkable efficiency, utilizing only 5-6% of a single CPU core while idle—a testament to the potential of a more optimized approach.

Qt, while undeniably powerful and widely used, seems to be trending towards increased complexity with each version. While it remains an excellent framework, there's a growing need for lighter, more focused alternatives.

These observations converged into a singular vision: creating a lightweight, efficient desktop application development approach that prioritizes performance, simplicity, and resource consciousness.

## Core Architecture

The library leverages the strengths of:
- **LVGL**: Provides a comprehensive widget system and UI management
- **Raylib**: Offers hardware-accelerated rendering and streamlined input event processing

## Project Scope

### Objectives
- Deliver an accessible framework for desktop GUI development
- Create a performant alternative to Electron-based applications
- Emphasize ease of use and minimal complexity

### Limitations
- Not designed for embedded system development
- Does not support extensive configuration scenarios

## Getting Started

### Prerequisites

#### System Dependencies
For Ubuntu/Debian-based systems, install the following build dependencies:

```sh
sudo apt install cmake build-essential libegl1-mesa-dev
```

### Build And Run The Example

1. Clone the repository:
```sh
git clone --recursive https://github.com/thaolt/lvgl_raylib.git
```

2. Build the example project:
```sh
cd lvgl_raylib
mkdir -p build && cd build
cmake .. && make -j
./lvgl_raylib_example/lvgl_raylib_example
```

### Quick Application Bootstrapping

Create a new project by copying and modifying the example:

```sh
git clone --recursive https://github.com/thaolt/lvgl_raylib.git myapp
cd myapp
rm -r .git
mv lvgl_raylib_example myapp
```

Update the root `CMakeLists.txt`:
```diff
-add_subdirectory(lvgl_raylib_example)
+add_subdirectory(myapp)
```

Build and run:
```sh
mkdir -p build && cd build
cmake ..
make -j
./myapp/myapp
```

## Code Example

Here's a concise demonstration of creating a simple UI:

```c
#include "raylib.h"
#include "lvgl.h"
#include "lvgl_raylib.h"

#define WIDTH 800
#define HEIGHT 600

void create_widgets() {
    // Create a text input for name
    lv_obj_t *name_ta = lv_textarea_create(lv_screen_active());
    lv_textarea_set_placeholder_text(name_ta, "Enter your name");
    lv_obj_set_width(name_ta, 250);
    lv_obj_align(name_ta, LV_ALIGN_CENTER, 0, -50);

    // Create an interactive button
    lv_obj_t *btn = lv_button_create(lv_screen_active());
    lv_obj_set_width(btn, 250);
    lv_obj_set_height(btn, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    // Add label to button
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click me");
    lv_obj_center(btn_label);
}

int main() {
    InitWindow(WIDTH, HEIGHT, "LVGL Raylib Example");
    SetTargetFPS(60);
    lvgl_raylib_init(WIDTH, HEIGHT);
    create_widgets();

    while (!WindowShouldClose()) {
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
```

## Contributing

We welcome and appreciate community contributions! Please follow these guidelines:

### Contribution Process
1. **Repository Setup**
   - Fork the project
   - Clone your forked repository
   - Create a descriptive branch (e.g., `feature/add-new-widget`)

2. **Development Best Practices**
   - Adhere to existing code style and conventions
   - Add or update tests for new functionality
   - Ensure all tests pass before submitting

3. **Pull Request Guidelines**
   - Provide a clear, descriptive title and description
   - Reference related issues
   - Be open to constructive feedback

### Code of Conduct
- Maintain a respectful and inclusive environment
- Offer constructive and supportive feedback
- Collaborate with empathy and professionalism

## License

This project is released under the MIT License. See the LICENSE file for complete details.

Copyright (c) 2025 thaolt
