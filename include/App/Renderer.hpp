#pragma once
#include <glfw/glfw3.h>
#include <memory>

#include "ECS/ECS.hpp"
#include "ECS/Components/Transform.hpp"
#include "App/Camera.hpp"
#include "Drawable.hpp"

class Renderer final {
    GLFWwindow* window;
    Camera* active_cam; // Currently active camera

    float aspect_ratio; // keep track of the aspect ratio
    bool dirty_cam = false;
public:
    Renderer(GLFWwindow* window);
    void Begin();
    void End();

    [[deprecated("The Render() function is not recommended to be used, use the Begin() and End() function instead!")]]
    void Render(entt::registry& r);
    void Update(float delta);
    void SetWindowAspectRatio(float ratio);
};