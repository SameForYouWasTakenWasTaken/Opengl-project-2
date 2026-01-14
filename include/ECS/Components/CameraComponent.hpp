#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraComponent {
    // Camera settings. Private because I need to recalculate the projection when these get changed and I could easily do that with the functions below
    float aspect;
    float fov = 45.f;
    float near = 0.05f;
    float far = 150.f;

    // Private to prevent bugs, such as grabbing the projection right after its fov changes (for example). You should recalculate it as soon as something of it changes
    glm::mat4 projection = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);
    
    void UpdateSettings();
public:
    CameraComponent(float aspect_ratio);

    bool dirty_proj = false;

    glm::vec3 cam_front = {0.f, 0.f, -1.f};
    glm::vec3 cam_up = {0.f, 1.f, 0.f};


    void SetView(const glm::mat4& matrix);
    void SetFOV(float n);
    void SetAspectRatio(float n);
    void SetNear(float n);
    void SetFar(float n);
    void LookAt(const glm::vec3& pos);

    glm::mat4 GetVP(); // Project * view mat4
    // void GetFOV() const;
    // void GetAspect() const;
    // void GetNear() const;
    // void GetFar() const;
};