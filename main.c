/*******************************************************************************************
* Project Title: Menu-Driven 2D Graphics Editor
* Subject: Advanced C Programming with Generative AI
* Description: A hardware-accelerated 2D graphics editor using C and Raylib.
* Features: Draw Shapes, Change Colors, Save to File, Load from File.
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SHAPES 1000

// --- 1. DATA STRUCTURES ---
// Enum to keep track of which tool is currently selected
typedef enum { SHAPE_LINE, SHAPE_CIRCLE, SHAPE_RECT, SHAPE_ELLIPSE } ShapeType;

// Struct to hold the properties of each drawn shape
typedef struct {
    ShapeType type;
    int x1, y1;     // Start point (or center)
    int x2, y2;     // End point (or width/height/radius)
    Color color;    // Raylib color object
} Shape;

// --- 2. GLOBAL STATE VARIABLES ---
Shape shapes[MAX_SHAPES];           // Array to store all our drawings
int shapeCount = 0;                 // Counter for how many shapes exist
ShapeType currentTool = SHAPE_LINE; // Default tool
Color currentColor = RED;           // Default color

// --- 3. FUNCTION DEFINITIONS ---

// Function to add a new shape to our array
void AddShape(ShapeType type, int x, int y, int w, int h, Color col) {
    if (shapeCount < MAX_SHAPES) {
        shapes[shapeCount].type = type;
        shapes[shapeCount].x1 = x;
        shapes[shapeCount].y1 = y;
        shapes[shapeCount].x2 = w;
        shapes[shapeCount].y2 = h;
        shapes[shapeCount].color = col;
        shapeCount++;
    }
}

// Function to save the shapes array to a binary file
void SaveDrawing(const char* filename) {
    FILE *file = fopen(filename, "wb"); // Open in Write-Binary mode
    if (file != NULL) {
        fwrite(&shapeCount, sizeof(int), 1, file); // Save the count first
        fwrite(shapes, sizeof(Shape), shapeCount, file); // Save the array data
        fclose(file);
    }
}

// Function to load the shapes array from a binary file
void LoadDrawing(const char* filename) {
    FILE *file = fopen(filename, "rb"); // Open in Read-Binary mode
    if (file != NULL) {
        fread(&shapeCount, sizeof(int), 1, file); // Read the count first
        fread(shapes, sizeof(Shape), shapeCount, file); // Read the array data
        fclose(file);
    }
}

// Function to draw the top menu/toolbar
void DrawUI() {
    // Draw Toolbar Background
    DrawRectangle(0, 0, 800, 45, LIGHTGRAY);
    DrawRectangleLines(0, 0, 800, 45, DARKGRAY);

    // Draw Instructions
    DrawText("TOOLS: 1:Line  2:Circle  3:Rect  4:Ellipse  |  ACTIONS: C:Color  S:Save  L:Load  X:Clear", 15, 12, 16, BLACK);
    
    // Determine tool name for status
    const char* toolName = "Unknown";
    switch(currentTool) {
        case SHAPE_LINE: toolName = "Line"; break;
        case SHAPE_CIRCLE: toolName = "Circle"; break;
        case SHAPE_RECT: toolName = "Rectangle"; break;
        case SHAPE_ELLIPSE: toolName = "Ellipse"; break;
    }

    // Draw Status Bar at bottom
    DrawRectangle(0, 565, 800, 35, LIGHTGRAY);
    DrawText(TextFormat("Current Tool: %s", toolName), 15, 575, 20, currentColor);
}

// --- 4. MAIN EXECUTABLE LOOP ---
int main(void) {
    // Initialize the Canvas Window (Width: 800, Height: 600)
    InitWindow(800, 600, "Menu-Driven 2D Graphics Editor");
    SetTargetFPS(60); // Run at 60 frames per second

    Vector2 startPos = {0};
    int isDrawing = 0; // Boolean flag to check if user is dragging the mouse

    // Main Game/App Loop
    while (!WindowShouldClose()) {
        
        // ==========================================
        // INPUT & LOGIC HANDLING (CONTROLLER)
        // ==========================================
        
        // 1. Keyboard Menu Handling (Switch-Case)
        int key = GetKeyPressed();
        switch(key) {
            case KEY_ONE:   currentTool = SHAPE_LINE; break;
            case KEY_TWO:   currentTool = SHAPE_CIRCLE; break;
            case KEY_THREE: currentTool = SHAPE_RECT; break;
            case KEY_FOUR:  currentTool = SHAPE_ELLIPSE; break;
            
            case KEY_C: // Cycle colors
                if (currentColor.r == 230) currentColor = BLUE;      // If Red, make Blue
                else if (currentColor.b == 255) currentColor = GREEN;// If Blue, make Green
                else currentColor = RED;                             // If Green, make Red
                break;
                
            case KEY_S: SaveDrawing("my_art.dat"); break;
            case KEY_L: LoadDrawing("my_art.dat"); break;
            case KEY_X: shapeCount = 0; break; // Clear screen by resetting array count
        }

        // 2. Mouse Handling
        // When left click is pressed down, record the starting coordinates
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMouseY() > 45 && GetMouseY() < 565) {
            startPos = GetMousePosition();
            isDrawing = 1;
        }
        
        // When left click is released, calculate final shape parameters and save to array
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isDrawing) {
            Vector2 endPos = GetMousePosition();
            isDrawing = 0;
            
            if (currentTool == SHAPE_LINE) {
                AddShape(SHAPE_LINE, startPos.x, startPos.y, endPos.x, endPos.y, currentColor);
            }
            else if (currentTool == SHAPE_CIRCLE) {
                int radius = (int)sqrt(pow(endPos.x - startPos.x, 2) + pow(endPos.y - startPos.y, 2));
                AddShape(SHAPE_CIRCLE, startPos.x, startPos.y, radius, 0, currentColor);
            }
            else if (currentTool == SHAPE_RECT) {
                AddShape(SHAPE_RECT, startPos.x, startPos.y, endPos.x - startPos.x, endPos.y - startPos.y, currentColor);
            }
            else if (currentTool == SHAPE_ELLIPSE) {
                AddShape(SHAPE_ELLIPSE, startPos.x, startPos.y, abs((int)endPos.x - (int)startPos.x), abs((int)endPos.y - (int)startPos.y), currentColor);
            }
        }

        // ==========================================
        // RENDERING (VIEW)
        // ==========================================
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear canvas with white background

        // Loop through array and draw all saved shapes
        for (int i = 0; i < shapeCount; i++) {
            Shape s = shapes[i];
            switch(s.type) {
                case SHAPE_LINE: 
                    DrawLine(s.x1, s.y1, s.x2, s.y2, s.color); 
                    break;
                case SHAPE_CIRCLE: 
                    DrawCircleLines(s.x1, s.y1, s.x2, s.color); 
                    break;
                case SHAPE_RECT: 
                    DrawRectangleLines(s.x1, s.y1, s.x2, s.y2, s.color); 
                    break;
                case SHAPE_ELLIPSE: 
                    DrawEllipseLines(s.x1, s.y1, s.x2, s.y2, s.color); 
                    break;
            }
        }

        // Draw a preview shape while the user is actively dragging the mouse
        if (isDrawing) {
            if (currentTool == SHAPE_LINE) DrawLine(startPos.x, startPos.y, GetMouseX(), GetMouseY(), LIGHTGRAY);
            if (currentTool == SHAPE_RECT) DrawRectangleLines(startPos.x, startPos.y, GetMouseX() - startPos.x, GetMouseY() - startPos.y, LIGHTGRAY);
        }

        // Draw the Menu UI on top of everything
        DrawUI();

        EndDrawing();
    }

    // Free memory and close OS window
    CloseWindow(); 
    return 0;
}