#include "App/Renderer.hpp"

Renderer::Renderer(GLFWwindow* window)
: window(window)
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
    if (active_cam)
        active_cam->SetAspectRatio(ratio);
}

void Renderer::Update(float delta)
{
    float speed = 5.f * delta;

    if (active_cam == nullptr) return;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        speed = 0.005f * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        active_cam->transform.Move({0.f, 0.f, -speed});
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        active_cam->transform.Move({-speed, 0.f, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        active_cam->transform.Move({0.f, 0.f, speed});
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        active_cam->transform.Move({speed, 0.f, 0.f});
    }
    // Up and down
    if (glfwGetKey(window, GLFW_KEY_E))
    {
        active_cam->transform.Move({0.f, speed, 0.f});
    }
    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        active_cam->transform.Move({0.f, -speed, 0.f});
    }
}

void Renderer::Render(entt::registry& r)
{
    auto view = r.view<Transform, Drawable>();
    auto cameras = r.view<Camera, Transform>();
    
    entt::entity active_camera_entity;

    cameras.each([&](auto entity, Camera& cam, Transform& t) {
        active_camera_entity = entity;
        return; // Only allow the first one
    });

    active_cam = &cameras.get<Camera>(active_camera_entity); 
    active_cam->SetFOV(45.f);

    view.each([this](auto entity, Transform& transform, Drawable& drawable){
        if (active_cam == nullptr) return;
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
        
        active_cam->Render();
        drawable.shader.UseProgram();
        drawable.shader.SetMatrix4("model", 1, glm::value_ptr(model));
        drawable.shader.SetMatrix4("VP_mat", 1, glm::value_ptr(active_cam->GetVP()));
        
        
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