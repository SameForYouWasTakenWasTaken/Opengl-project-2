#pragma once
#include <memory>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ECS/Components/Transform.hpp"

class Camera final {
private:

    mutable glm::mat4 projection;
    glm::mat4 view;
    
    mutable bool dirty_proj = false;
    // Camera settings
    float fov;
    float aspect;
    float near;
    float far;
    void UpdateSettings() const;

    
public:

    Camera(Transform& t, float _aspect, float _fov = 45.f, float _near = 1, float _far = 100);


    void SetFOV(float n);
    void SetAspectRatio(float n);
    void SetNear(float n);
    void SetFar(float n);

    void LookAt(const glm::vec3& pos);

    glm::mat4 GetVP() const; //View, Projection
    // void GetView() const;
    const glm::vec3& GetPosition() const;
    // void GetFOV() const;
    // void GetAspect() const;
    // void GetNear() const;
    // void GetFar() const;

    void Render();

    Transform transform;
    glm::vec3 cam_front = {0.f, 0.f, -1.f};
    glm::vec3 cam_up = {0.f, 1.f, 0.f};

};
