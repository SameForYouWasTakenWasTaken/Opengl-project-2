#pragma once
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera final {
private:
    glm::vec3 cam_pos = {0.f, 0.f, 0.f};
    glm::vec3 cam_front = {0.f, 0.f, -1.f};
    glm::vec3 cam_up = {0.f, 1.f, 0.f};

    mutable glm::mat4 projection;
    glm::mat4 view;

    // Camera settings
    float fov;
    float aspect;
    float near;
    float far;

    mutable bool dirty_proj = false;

    void UpdateSettings() const;
public:

    Camera(float _fov, float _aspect, float _near = 1, float _far = 100);


    void SetFOV(float n);
    void SetAspectRatio(float n);
    void SetNear(float n);
    void SetFar(float n);

    void Move(const glm::vec3& increment);
    void SetPosition(const glm::vec3& pos);
    void LookAt(const glm::vec3& pos);

    glm::mat4 GetVP() const; //View, Projection
    // void GetView() const;
    glm::vec3 GetPosition() const;
    // void GetFOV() const;
    // void GetAspect() const;
    // void GetNear() const;
    // void GetFar() const;

    void Render();
};
