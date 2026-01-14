#include "App/Renderer.hpp"

Renderer::Renderer(GLFWwindow* window, entt::registry& registry)
: window(window), CamSystem(std::make_unique<CameraSystem>(registry, dispatcher))
{
}

void Renderer::Begin()
{

}

void Renderer::End()
{

}

void Renderer::SetWindowAspectRatio(float ratio)
{
    if (CamSystem->active_cam_entity != entt::null)
    {
        dirty_cam = true;
    }
}

void Renderer::Update(entt::registry& registry, float delta)
{
    if (CamSystem->active_cam_entity == entt::null) 
    {
        spdlog::error("Renderer::Update() returned an error! : Camera system' active camera is null!");
        return;
    };
    
    if (!registry.valid(CamSystem->active_cam_entity))
    {
        spdlog::warn("Renderer::Update() returned a warning! : Camera system' active camera is NOT apart of this registry!");
        return;
    }

    float speed = 5.f * delta;
    auto& transform = registry.get<Transform>(CamSystem->active_cam_entity);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        speed = 0.005f * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        transform.Move({0.f, 0.f, -speed});
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        transform.Move({-speed, 0.f, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        transform.Move({0.f, 0.f, speed});
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        transform.Move({speed, 0.f, 0.f});
    }
    // Up and down
    if (glfwGetKey(window, GLFW_KEY_E))
    {
        transform.Move({0.f, speed, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        transform.Move({0.f, -speed, 0.f});
    }
}

void Renderer::Render(entt::registry& registry)
{
    CamSystem->RenderSystem(registry); // Already run the render system to load everything in, dont move it under the if statements

    auto view = registry.view<Transform, Drawable>();
    
    if (CamSystem->active_cam_entity == entt::null) 
    {
        spdlog::error("Renderer::Render() returned an error! : Camera system' active camera is null!");
        return;
    };
    
    if (!registry.valid(CamSystem->active_cam_entity))
    {
        spdlog::warn("Renderer::Render() returned a warning! : Camera system' active camera is NOT apart of this registry!");
        return;
    }

    auto& cc = registry.get<CameraComponent>(CamSystem->active_cam_entity);

    if (dirty_cam)
        cc.SetAspectRatio(aspect_ratio);

    dirty_cam = false;

    view.each([this, &cc](auto entity, Transform& transform, Drawable& drawable){
        std::vector<Vertex> vertices = drawable.GetVertices();
        std::vector<GLuint> indices = drawable.GetIndices();
        
        
        if (drawable.dirty_buffers) {
            drawable.vao.Bind();
            drawable.vbo.Bind();
            
            drawable.vbo.SetData(vertices, drawable.drawState);
            if (drawable.drawMode == DrawMode::Elements && drawable.dirty_indices) {
                drawable.ebo.Bind();
                drawable.ebo.SetData(indices, drawable.drawState);
            }
            
            // link vertex attributes here once
            drawable.dirty_buffers = false;
            drawable.dirty_indices = false;
        }
        
        glm::mat4 model(1.f);
        
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, glm::radians(transform.rotation.x), {1,0,0});
        model = glm::rotate(model, glm::radians(transform.rotation.y), {0,1,0});
        model = glm::rotate(model, glm::radians(transform.rotation.z), {0,0,1});
        model = glm::scale(model, transform.scale);


        drawable.shader.UseProgram();
        drawable.shader.SetMatrix4("model", 1, glm::value_ptr(model));
        drawable.shader.SetMatrix4("VP_mat", 1, glm::value_ptr(cc.GetVP()));
        
        
        if (drawable.using_texture)
            drawable.texture2D.Use();
        drawable.vao.Bind();

        if (drawable.drawMode == DrawMode::Elements) {
            glDrawElements(drawable.primitive,
                        static_cast<GLsizei>(indices.size()),
                        GL_UNSIGNED_INT,
                        nullptr);

        } else if(drawable.drawMode == DrawMode::Arrays) {
            glDrawArrays(drawable.primitive,
                        0,
                        static_cast<GLsizei>(vertices.size()));
        }
    });
}