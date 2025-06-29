/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** raylib_wrapper.cpp
*/
#include "raylib_wrapper.hpp"

RaylibWrapper::RaylibWrapper(int screenWidth, int screenHeight, const char* title) {
    _screenHeight = screenHeight;
    _screenWidth = screenWidth;
    _title = title;

    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);
}

RaylibWrapper::~RaylibWrapper() {
    CloseWindow();
}

void RaylibWrapper::BeginDrawing() {
    ::BeginDrawing();
}

void RaylibWrapper::EndDrawing() {
    ::EndDrawing();
}

void RaylibWrapper::ClearBackground(Color color) {
    ::ClearBackground(color);
}

bool RaylibWrapper::WindowShouldClose() {
    return ::WindowShouldClose();
}

void RaylibWrapper::CloseWindow() {
    ::CloseWindow();
}

int RaylibWrapper::GetScreenWidth() {
    return ::GetScreenWidth();
}

int RaylibWrapper::GetScreenHeight() {
    return ::GetScreenHeight();
}

void RaylibWrapper::DrawRectangle(int posX, int posY, int width, int height, Color color) {
    ::DrawRectangle(posX, posY, width, height, color);
}

void RaylibWrapper::DrawText(const char* text, int posX, int posY, int fontSize, Color color) {
    ::DrawText(text, posX, posY, fontSize, color);
}

void RaylibWrapper::DrawCube(Vector3 position, float width, float height, float length, Color color) {
    ::DrawCube(position, width, height, length, color);
}

void RaylibWrapper::BeginMode3D(Camera3D camera) {
    ::BeginMode3D(camera);
}

void RaylibWrapper::EndMode3D() {
    ::EndMode3D();
}

bool RaylibWrapper::CheckCollisionRayBox(Ray ray, BoundingBox box) {
    float tmin = (box.min.x - ray.position.x) / ray.direction.x;
    float tmax = (box.max.x - ray.position.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.min.y - ray.position.y) / ray.direction.y;
    float tymax = (box.max.y - ray.position.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (box.min.z - ray.position.z) / ray.direction.z;
    float tzmax = (box.max.z - ray.position.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;
    return ((tmin < 0.0f) && (tmax > 0.0f)) || (tmin >= 0.0f);
}

void RaylibWrapper::SetTargetFPS(int fps) {
    ::SetTargetFPS(fps);
}

Model RaylibWrapper::LoadModel(const char* fileName) {
    return ::LoadModel(fileName);
}

void RaylibWrapper::DrawModel(Model model, Vector3 position, float scale, Color tint) {
    ::DrawModel(model, position, scale, tint);
}

void RaylibWrapper::UnloadModel(Model model) {
    ::UnloadModel(model);
}

Ray RaylibWrapper::GetMouseRay(Vector2 mousePosition, Camera camera) {
    return ::GetMouseRay(mousePosition, camera);
}

RayCollision RaylibWrapper::GetRayCollisionBox(Ray ray, BoundingBox box) {
    return ::GetRayCollisionBox(ray, box);
}

void RaylibWrapper::UpdateModelAnimation(Model model, ModelAnimation anim, int frame) {
    ::UpdateModelAnimation(model, anim, frame);
}

ModelAnimation* RaylibWrapper::LoadModelAnimations(const char* fileName, int* animCount) {
    return ::LoadModelAnimations(fileName, animCount);
}

void RaylibWrapper::DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) {
    ::DrawModelEx(model, position, rotationAxis, rotationAngle, scale, tint);
}

void RaylibWrapper::DrawModelWires(Model model, Vector3 position, float scale, Color tint) {
    ::DrawModelWires(model, position, scale, tint);
}

void RaylibWrapper::DrawCubeWires(Vector3 position, float width, float height, float length, Color color) {
    ::DrawCubeWires(position, width, height, length, color);
}

bool RaylibWrapper::IsKeyDown(int key) {
    return ::IsKeyDown(key);
}

void RaylibWrapper::DrawEllipse(float x, float y, float radiusX, float radiusY, Color color) {
    ::DrawEllipse(x, y, radiusX, radiusY, color);
}

void RaylibWrapper::DrawSphere(Vector3 centerPos, float radius, Color color) {
    ::DrawSphere(centerPos, radius, color);
}

Color RaylibWrapper::GetColor(int hexValue) {
    return ::GetColor(hexValue);
}

void RaylibWrapper::DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint) {
    ::DrawTextureEx(texture, position, rotation, scale, tint);
}

bool RaylibWrapper::CheckCollisionPointRec(Vector2 point, Rectangle rec) {
    return ::CheckCollisionPointRec(point, rec);
}

bool RaylibWrapper::IsMouseButtonPressed(int button) {
    return ::IsMouseButtonPressed(button);
}

void RaylibWrapper::InitAudioDevice() {
    ::InitAudioDevice();
}

Texture2D RaylibWrapper::LoadTexture(const char* fileName) {
    return ::LoadTexture(fileName);
}

Music RaylibWrapper::LoadMusicStream(const char* fileName) {
    return ::LoadMusicStream(fileName);
}

Vector2 RaylibWrapper::GetMousePosition() {
    return ::GetMousePosition();
}

void RaylibWrapper::UpdateCamera(Camera* camera, int mode) {
    ::UpdateCamera(camera, mode);
}

void RaylibWrapper::UnloadTexture(Texture2D texture) {
    ::UnloadTexture(texture);
}

void RaylibWrapper::UnloadMusicStream(Music music) {
    ::UnloadMusicStream(music);
}
