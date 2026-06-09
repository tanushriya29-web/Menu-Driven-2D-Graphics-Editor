# Menu-Driven 2D Graphics Editor

A high-performance, hardware-accelerated 2D vector graphics editor built from scratch using pure C (C99 standard) and the Raylib graphics engine. This application provides a modern, GPU-accelerated OpenGL pipeline running fluidly at 60 FPS, completely replacing legacy, CPU-bound dependencies like graphics.h.

Developed as a mini-project for Advanced C Programming with Generative AI, this project showcases structural data abstraction, state-machine event handling, and low-level binary file serialization.

---

## Core Technical Features

* GPU-Accelerated Rendering: Uses standard OpenGL wrappers via Raylib to handle real-time geometric rasterization instantly without taxing the host CPU.
* State-Driven Tool Pipeline: Implements an interactive Model-View-Controller (MVC) style input processing system to seamlessly transition between drawing tools.
* Persistent Binary Storage: Rather than using heavy text formats like JSON or CSV, it writes raw memory structs directly to disk (my_art.dat) using optimized C binary file streams (fwrite and fread).
* Interactive Live Preview: Features a real-time tracking buffer that renders temporary shapes dynamically as the user drags the mouse, pushing them to the global stack only upon release.

---

## Workspace Controls and Shortcuts

The editor uses a clean, top-docked menu bar for user guide instructions and a bottom-docked status indicator showcasing the active tool and color selection.

| Shortcut Key | Action Category | Functional Operation |
| :--- | :--- | :--- |
| 1 | Tool Selection | Switch active primitive tool to Line mode |
| 2 | Tool Selection | Switch active primitive tool to Circle mode |
| 3 | Tool Selection | Switch active primitive tool to Rectangle mode |
| 4 | Tool Selection | Switch active primitive tool to Ellipse mode |
| C | Configuration | Cycle stroke color dynamically (Red to Blue to Green) |
| S | File I/O Stream | Serialize full canvas memory cache to local binary file (my_art.dat) |
| L | File I/O Stream | Deserialize and rebuild the shape coordinate stack from disk |
| X | Memory Reset | Flush canvas cache completely (Clear Screen) |
| Left Mouse Drag | Canvas Drawing | Draw active geometric object using anchor coordinates |

---

## Structural Data Architecture

The backend handles modern data encapsulation using standard C structure types and enum flags to map object attributes inside a static array buffer.

```c
// Strongly typed enum tracking selected tool states
typedef enum { 
    SHAPE_LINE, 
    SHAPE_CIRCLE, 
    SHAPE_RECT, 
    SHAPE_ELLIPSE 
} ShapeType;

// Encapsulated struct representing complete geometric identity
typedef struct {
    ShapeType type;  // Shape variant selector
    int x1, y1;      // Spatial coordinate origin
    int x2, y2;      // Terminal boundaries / radius metrics
    Color color;     // Direct RGB byte flag for GPU binding
} Shape;
