#include <spdlog/spdlog.h>
#include "App/App.hpp"
#include "Debug.hpp"
#include "LowLevelShit/Shader.hpp"
#include "LowLevelShit/VAO.hpp"
#include "LowLevelShit/VBO.hpp"
#include "LowLevelShit/EBO.hpp"
#include "LowLevelShit/Texture2D.hpp"
#include "Renderable.hpp"
#include "App/Camera.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

App::App(AppSettings _AppSettings) : m_Settings(_AppSettings)
{
    if (!glfwInit())
        return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    m_Window = glfwCreateWindow(m_Settings.WindowHeight, m_Settings.WindowWidth, m_Settings.AppName.c_str(), NULL, NULL);
    
    if (!m_Window)
    {
        spdlog::error("Couldn't create window!");
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(m_Window, this);
    glfwMakeContextCurrent(m_Window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
        return;

	glfwSetFramebufferSizeCallback(m_Window, frame_buffer_size_callback);    

    enableReportGlErrors(); // Enable the DebugMessageCallback() function OpenGL provides since version 4,3.
    glfwSwapInterval(0); // No VSync. 1 = VSync
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Blending alpha thingy. Basically lets stuff be opaque or not
    

    // Standard creation

    float fov = 45.f;
    float aspect_ratio = m_Settings.WindowHeight / m_Settings.WindowWidth;

    m_Camera = std::make_shared<Camera>(fov, aspect_ratio, 0.001f);
    m_Renderer = std::make_unique<Renderer>(m_Window, m_Camera);
}

void App::Run()
{
    // std::vector<Vertex> vertices = {
    //     {{0.5f,  .5f, 0.0f}, {1.f, 0.f, 0.f, 1.f}, {1.f, 1.f}}, // top  right
    //     {{.5f, -.5f, .0f}, {0.f, 1.f, 0.f, 1.f}, {1.f, 0.f}}, // bottom right
    //     {{-.5f, -.5f, .0f}, {0.f,0.f,1.f,1.f}, {0.f, 0.f}},  // bottom left

    // }; 

    std::vector<Vertex> vertices = {
            {{0.5f,  .5f, 0.0f}, {1.f, 0.f, 0.f, 1.f}, {1,1}}, // top  right
            {{.5f, -.5f, .0f}, {0.f, 1.f, 0.f, 1.f}, {1,0}}, // bottom right
            {{-.5f, -.5f, .0f}, {0.f,0.f,1.f,1.f}, {0,0}},  // bottom left
            {{-.5f, .5f, .0f}, {0.f,0.f,1.f,1.f}, {0,1}},  // top left
     }; 

    std::vector<GLuint> indices = {
        0,1,2,
        3,2,0
    };
    Shader shader = Shader("Shaders/standard_texture.frag", "Shaders/standard_texture.vert");

    Texture2D texture = Texture2D("Resources/wall.jpg");
    Texture2D texture2 = Texture2D("Resources/placeholder.png");

    Renderable r(vertices, indices, shader, texture);
    Renderable s(vertices, indices, shader, texture2);
    r.RecreateTexture2D("Resources/abc.png");

    m_Camera->SetPosition({0.f, 0.f, 3.f});
    m_Camera->LookAt({0.f, 0.f, 0.f});
    r.SetPosition({0.f, 0.f, 0.f});
    
    s.SetPosition({3.f, 0.f, 0.f});
    s.SetScale({1.f,1.f,1.f});
    s.Rotate({0.f, 0.f, -90.f});

    double lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(m_Window))
    {        
        // recalculate delta time
        double currentFrame = glfwGetTime();
        float dt = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Renderer->Update(dt);

        m_Renderer->Begin();

        auto shader_a = r.GetShader();
        auto shader_b = s.GetShader();

        m_Camera->Render();
        
        shader_a.UseProgram();
        shader_a.SetMatrix4("VP_mat", 1, glm::value_ptr(m_Camera->GetVP()));
        
        shader_b.UseProgram();
        shader_b.SetMatrix4("VP_mat", 1, glm::value_ptr(m_Camera->GetVP()));
        
        r.Draw();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

        s.Draw();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

        m_Renderer->End();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void App::Update()
{
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);

    m_Settings.WindowHeight = height;
    m_Settings.WindowWidth = width;

    m_Camera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, 
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height));
    auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

    if (app)
    {
        app->Update();
    }
    
}