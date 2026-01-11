#pragma once
#include <glfw/glfw3.h>
#include <glad/gl.h>
#include <memory>

#include "App/AppSettings.hpp"
#include "App/Renderer.hpp"
#include "App/Camera.hpp"


static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

class App final {
    std::unique_ptr<Renderer> m_Renderer;
    AppSettings m_Settings;

    GLFWwindow* m_Window;
    
public:
    App(AppSettings _AppSettings);
    void Run();
    void Update(); // Update its contents
};