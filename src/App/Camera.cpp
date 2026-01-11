#include "App/Camera.hpp"

Camera::Camera(Transform& t, float _aspect, float _fov, float _near, float _far)
: fov(_fov), aspect(_aspect), near(_near), far(_far), transform(t)
{
    view = glm::mat4(1.f);
    projection = glm::mat4(1.f);

    view = glm::translate(view, glm::vec3(0.f, 0.f, 0.f));
    projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::SetFOV(float n)
{
    fov = n;
    dirty_proj = true;
}

void Camera::SetAspectRatio(float n)
{
    aspect = n;
    dirty_proj = true;
}

void Camera::SetNear(float n)
{
    near = n;
    dirty_proj = true;
}

void Camera::SetFar(float n)
{
    far = n;
    dirty_proj = true;
}

void Camera::LookAt(const glm::vec3& target)
{
    cam_front = glm::normalize(target - transform.position);
}

glm::mat4 Camera::GetVP() const
{
    UpdateSettings();
    return projection * view;
}

void Camera::Render()
{
    UpdateSettings();
    view = glm::lookAt(transform.position, transform.position + cam_front, cam_up);
}

void Camera::UpdateSettings() const
{
    if (dirty_proj)
        projection = glm::perspective(glm::radians(fov), aspect, near, far);

    dirty_proj = false;
}