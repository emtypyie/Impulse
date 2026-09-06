#include "raylib.h"
#include "vector3d.hpp"
int main() {
    // 1. Setup the application window dimensions
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Impulse");
    SetTargetFPS(90);

    // 2. Initialize Raylib's 3D Perspective Camera Setup
    Camera3D camera = { 0 };
    camera.position = Vector3{30.0f, 20.0f, 30.0f}; // Placed offset to see all axes clearly
    camera.target = Vector3{0.0f, 0.0f, 0.0f};      // Looking dead-center at the origin
    camera.up = Vector3{0.0f, 1.0f, 0.0f};          // Defines positive Y as the "Up" vector
    camera.fovy = 60.0f;                             // Field of View perspective angle
    camera.projection = CAMERA_PERSPECTIVE;          // Enables true 3D depth perception

    // Capture the mouse cursor inside the screen window for smooth free-look controls
    DisableCursor();

    // 3. The Application Execution Loop
    while (!WindowShouldClose()) {
        
        // Update the 3D free-flying camera controller inputs:
        // - Mouse movement rotates your point of view
        // - WASD keys let you fly forward, left, backward, right
        // - E key flies Up, Q key flies Down
        UpdateCamera(&camera, CAMERA_FREE);

        // --- RENDERING PHASE ---
        BeginDrawing();
            ClearBackground(BLACK); // Cosmic space background

            // Shift the graphics card pipeline from 2D screen coordinates to 3D space matrix operations
            BeginMode3D(camera);
                
                // Draw a standard floor grid on the flat XZ plane for spatial reference
                // (Slices: 20, Spacing between slices: 5.0 units)
                DrawGrid(20, 5.0f);

                // --- DRAW THE CORE ORIGIN LINES (RGB Rule) ---
                const float axisLength = 50.0f;

                // Red Line: The X-Axis Vector going from (0,0,0) outwards right
                DrawLine3D(Vector3{0.0f, 0.0f, 0.0f}, Vector3{axisLength, 0.0f, 0.0f}, RED);
                
                // Green Line: The Y-Axis Vector going from (0,0,0) outwards up
                DrawLine3D(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, axisLength, 0.0f}, GREEN);
                
                // Blue Line: The Z-Axis Vector going from (0,0,0) outwards forward
                DrawLine3D(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, axisLength}, BLUE);

                // Draw a small distinct white indicator sphere precisely at the true origin point (0,0,0)
                DrawSphere(Vector3{0.0f, 0.0f, 0.0f}, 0.2f, WHITE);

            EndMode3D(); // Close 3D rendering context; UI layers below are painted flat on top

            // --- 2D HEADS-UP DISPLAY (HUD) OVERLAY ---
            DrawFPS(10, 10);
            
            // Render a clear on-screen color key guide for axis recognition
            DrawText("Orbital Mechanics", 10, 35, 20, LIGHTGRAY);
            DrawText("X-AXIS ", 10, 65, 16, RED);
            DrawText("Y-AXIS ", 10, 85, 16, GREEN);
            DrawText("Z-AXIS ", 10, 105, 16, BLUE);
            DrawText("Engineered By EMTYPYIE", 10, screenHeight - 30, 16, PINK);

        EndDrawing();
    }

    // Release mouse hardware hold and close the window pipeline gracefully
    EnableCursor();
    CloseWindow();
    return 0;
}
