#pragma once
#include <glfw/glfw3.h>
#include <memory>
#include "App/Camera.hpp"
#include "Renderable.hpp"

class Renderer final {
    GLFWwindow* window;
    std::shared_ptr<Camera> camera; // Main camera
public:
    Renderer(GLFWwindow* window, std::shared_ptr<Camera> cam);
    void Begin();
    void End();

    [[deprecated("The Render() function is not recommended to be used, use the Begin() and End() function instead!")]]
    void Render();
    void Update(float delta);
};