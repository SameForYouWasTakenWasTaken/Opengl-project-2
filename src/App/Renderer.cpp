#include "App/Renderer.hpp"

Renderer::Renderer(GLFWwindow* window, std::shared_ptr<Camera> cam)
: window(window), camera(cam)
{
    
}

void Renderer::Begin()
{

}

void Renderer::End()
{

}

void Renderer::Update(float delta)
{
    float speed = 5.f * delta;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        speed = 0.005f * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        camera->Move({0.f, 0.f, -speed});
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        camera->Move({-speed, 0.f, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        camera->Move({0.f, 0.f, speed});
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        camera->Move({speed, 0.f, 0.f});
    }
    // Up and down
    if (glfwGetKey(window, GLFW_KEY_E))
    {
        camera->Move({0.f, speed, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        camera->Move({0.f, -speed, 0.f});
    }

    auto pos = camera->GetPosition();
    spdlog::info("{}, {}, {}", pos.x, pos.y, pos.z);
}

void Renderer::Render()
{

}