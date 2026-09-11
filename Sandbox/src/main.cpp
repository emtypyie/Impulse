#include "raylib.h"
#include "vector3d.hpp"
#include "physics.hpp"
#include <cmath>

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Impulse - Massive Scale Solar System");
    SetTargetFPS(90);

    Camera3D camera = { 0 };
    camera.position = Vector3{250.0f, 300.0f, 250.0f}; 
    camera.target = Vector3{0.0f, 0.0f, 0.0f};      
    camera.up = Vector3{0.0f, 1.0f, 0.0f};          
    camera.fovy = 45.0f;                             
    camera.projection = CAMERA_PERSPECTIVE;          

    DisableCursor();

    PhysicsEngine physics;

    // --- MASSIVE SCALE CONFIGURATION (1 AU = 216.0 Units) ---
    physics.spawnBody(Body("Sun", Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), 1.0, 14.0, true));
    physics.spawnBody(Body("Mercury", Vector3d(-84.24, 0.0, 0.0), Vector3d(0.0, 0.0, -0.12), 0.00000016, 2.7, false));
    physics.spawnBody(Body("Venus", Vector3d(-155.52, 0.0, 0.0), Vector3d(0.0, 0.0, -0.08), 0.0000024, 4.0, false));
    physics.spawnBody(Body("Earth", Vector3d(-216.0, 0.0, 0.0), Vector3d(0.0, 0.0, -0.068), 0.000003, 4.5, false));
    physics.spawnBody(Body("Mars", Vector3d(-328.32, 0.0, 0.0), Vector3d(0.0, 0.0, -0.055), 0.00000032, 3.6, false));

    float targetDist = 600.0f; 
    float angleXZ = 0.785f;   
    float angleY = 0.8f;   

    float timeScale = 40.0f; 
    const float maxTimeScale = 200.0f; 
    Rectangle sliderBar = { 110, 140, 200, 10 };
    bool isDraggingSlider = false;
    bool cursorUnlocked = false;

    while (!WindowShouldClose()) {
        
        physics.step(GetFrameTime() * timeScale);

        if (IsKeyPressed(KEY_LEFT_ALT)) {
            cursorUnlocked = !cursorUnlocked;
            if (cursorUnlocked) EnableCursor(); else DisableCursor();
        }

        Vector2 mousePos = GetMousePosition();
        if (cursorUnlocked) {
            Rectangle clickZone = { sliderBar.x - 10, sliderBar.y - 10, sliderBar.width + 20, sliderBar.height + 20 };
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, clickZone)) isDraggingSlider = true;
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) isDraggingSlider = false;
            if (isDraggingSlider) {
                float newX = mousePos.x - sliderBar.x;
                if (newX < 0) newX = 0;
                if (newX > sliderBar.width) newX = sliderBar.width;
                timeScale = (newX / sliderBar.width) * maxTimeScale;
            }
        }

        if (!cursorUnlocked) {
            float scroll = GetMouseWheelMove();
            if (scroll != 0.0f) {
                targetDist -= scroll * 25.0f; 
                if (targetDist < 40.0f)   targetDist = 40.0f;   
                if (targetDist > 1500.0f) targetDist = 1500.0f; 
            }
            Vector2 mouseDelta = GetMouseDelta();
            angleXZ -= mouseDelta.x * 0.003f;
            angleY  += mouseDelta.y * 0.003f;
            if (angleY > 1.4f) angleY = 1.4f; if (angleY < -1.4f) angleY = -1.4f;
        }

        Vector3 viewForward = { -sinf(angleXZ), 0.0f, -cosf(angleXZ) };
        Vector3 viewRight   = { -cosf(angleXZ), 0.0f,  sinf(angleXZ) };
        float baseMoveSpeed = 200.0f; 
        if (IsKeyDown(KEY_LEFT_CONTROL)) baseMoveSpeed = 600.0f; 

        Vector3 moveDelta = { 0.0f, 0.0f, 0.0f };
        if (IsKeyDown(KEY_W)) { moveDelta.x += viewForward.x; moveDelta.z += viewForward.z; }
        if (IsKeyDown(KEY_S)) { moveDelta.x -= viewForward.x; moveDelta.z -= viewForward.z; }
        if (IsKeyDown(KEY_D)) { moveDelta.x -= viewRight.x;   moveDelta.z -= viewRight.z; } 
        if (IsKeyDown(KEY_A)) { moveDelta.x += viewRight.x;   moveDelta.z += viewRight.z; } 
        if (IsKeyDown(KEY_SPACE))      { camera.target.y += baseMoveSpeed * GetFrameTime(); }
        if (IsKeyDown(KEY_LEFT_SHIFT)) { camera.target.y -= baseMoveSpeed * GetFrameTime(); }

        camera.target.x += moveDelta.x * baseMoveSpeed * GetFrameTime();
        camera.target.z += moveDelta.z * baseMoveSpeed * GetFrameTime();
        camera.position.x = camera.target.x + targetDist * sinf(angleXZ) * cosf(angleY);
        camera.position.y = camera.target.y + targetDist * sinf(angleY);
        camera.position.z = camera.target.z + targetDist * cosf(angleXZ) * cosf(angleY);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                
                // CLEAN GRID: 20 Slices, 50 Units apart = 1000 unit width
                // Creates a much wider, cleaner floor layout
                DrawGrid(20, 50.0f); 

                DrawLine3D(Vector3{0,0,0}, Vector3{400,0,0}, RED);   
                DrawLine3D(Vector3{0,0,0}, Vector3{0,400,0}, GREEN); 
                DrawLine3D(Vector3{0,0,0}, Vector3{0,0,400}, BLUE);  

                for (const auto& body : physics.getBodies()) {
                    if (body.pathHistory.size() < 2) continue;
                    Color traceColor = VIOLET;
                    if (body.name == "Earth")   traceColor = SKYBLUE;
                    if (body.name == "Mars")    traceColor = ORANGE;
                    if (body.name == "Mercury") traceColor = DARKGRAY;
                    if (body.name == "Venus")   traceColor = GOLD; 
                    
                    for (size_t k = 0; k < body.pathHistory.size() - 1; ++k) {
                        Vector3 start = { (float)body.pathHistory[k].x, (float)body.pathHistory[k].y, (float)body.pathHistory[k].z };
                        Vector3 end   = { (float)body.pathHistory[k+1].x, (float)body.pathHistory[k+1].y, (float)body.pathHistory[k+1].z };
                        DrawLine3D(start, end, traceColor);
                    }
                }

                for (const auto& body : physics.getBodies()) {
                    Vector3 renderPos = { (float)body.position.x, (float)body.position.y, (float)body.position.z };
                    Color coreColor = LIGHTGRAY;
                    if (body.name == "Sun")    coreColor = GOLD;
                    if (body.name == "Earth")  coreColor = BLUE;
                    if (body.name == "Mars")   coreColor = RED;
                    if (body.name == "Venus")  coreColor = BEIGE;
                    
                    DrawSphere(renderPos, (float)body.radius, coreColor);
                    DrawSphereWires(renderPos, (float)body.radius + 0.2f, 16, 16, Fade(DARKGRAY, 0.5f));
                }
            EndMode3D();

            DrawRectangleRec(sliderBar, DARKGRAY);
            float handleX = sliderBar.x + (timeScale / maxTimeScale) * sliderBar.width;
            DrawRectangle(handleX - 6, sliderBar.y - 5, 12, 20, isDraggingSlider ? LIME : RAYWHITE);
            DrawText("Sim Speed:", 10, 137, 16, LIGHTGRAY);
            DrawText(TextFormat("%.2fx", timeScale), sliderBar.x + sliderBar.width + 15, 137, 16, LIME);
            DrawFPS(10, 10);
            DrawText("Massive Scale Physics", 10, 35, 20, GOLD);

        EndDrawing();
    }
    EnableCursor();
    CloseWindow();
    return 0;
}
