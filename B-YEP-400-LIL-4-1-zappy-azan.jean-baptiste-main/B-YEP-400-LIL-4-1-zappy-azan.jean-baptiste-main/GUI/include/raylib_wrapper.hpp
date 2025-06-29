/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** raylib_wrapper.hpp
*/

#pragma once

#include "raylib.h"
#include "raymath.h"
#include <memory>

class RaylibWrapper {
public:
    RaylibWrapper(int screenWidth, int screenHeight, const char* title);
    ~RaylibWrapper();

    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);
    bool WindowShouldClose();
    void CloseWindow();

    int GetScreenWidth();
    int GetScreenHeight();

    void DrawRectangle(int posX, int posY, int width, int height, Color color);
    void DrawText(const char* text, int posX, int posY, int fontSize, Color color);
    void DrawCube(Vector3 position, float width, float height, float length, Color color);
    void BeginMode3D(Camera3D camera);
    void EndMode3D();
    void SetTargetFPS(int fps);
    Model LoadModel(const char* fileName);
    void DrawModel(Model model, Vector3 position, float scale, Color tint);
    void UnloadModel(Model model);
    Ray GetMouseRay(Vector2 mousePosition, Camera3D camera);
    RayCollision GetRayCollisionBox(Ray ray, BoundingBox box);
    void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);
    ModelAnimation *LoadModelAnimations(const char* fileName, int* animCount);

    bool CheckCollisionRayBox(Ray ray, BoundingBox box);

    void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint);
    void DrawModelWires(Model model, Vector3 position, float scale, Color tint);
    void DrawCubeWires(Vector3 position, float width, float height, float length, Color color);
    bool IsKeyDown(int key);
    void DrawEllipse(float x, float y, float radiusX, float radiusY, Color color);
    void DrawSphere(Vector3 position, float radius, Color color);
    Color GetColor(int hexValue);
    void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
    bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
    bool IsMouseButtonPressed(int button);
    void InitAudioDevice();
    Texture2D LoadTexture(const char* fileName);
    Music LoadMusicStream(const char* fileName);
    Vector2 GetMousePosition();
    void UpdateCamera(Camera3D *camera, int mode);
    void UnloadTexture(Texture2D texture);
    void UnloadMusicStream(Music music);
private:
    int _screenWidth;
    int _screenHeight;
    const char* _title;
};
