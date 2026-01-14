#pragma once
#include <glfw/glfw3.h>
#include <memory>

#include "ECS/ECS.hpp"
#include "ECS/Components/Transform.hpp"
#include "App/CameraSystem.hpp"
#include "Drawable.hpp"

class Renderer final {
    GLFWwindow* window;

    entt::dispatcher dispatcher;
    std::unique_ptr<CameraSystem> CamSystem;

    float aspect_ratio = 1.f; // keep track of the aspect ratio
    bool dirty_cam = false;
public:
    Renderer(GLFWwindow* window, entt::registry& registry);
    void Begin();
    void End();

    void Render(entt::registry& registry);
    void Update(entt::registry& registry, float delta);
    void SetWindowAspectRatio(float ratio);
};