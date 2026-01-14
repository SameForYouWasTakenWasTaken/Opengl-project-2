#include "ECS/Components/CameraComponent.hpp"

CameraComponent::CameraComponent(float aspect_ratio)
: aspect(aspect_ratio)
{
    view = glm::translate(view, glm::vec3(0.f, 0.f, 0.f)); // Set to point 0,0,0
    projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

void CameraComponent::SetFOV(float n)
{
    fov = n;
    dirty_proj = true;
}

void CameraComponent::SetAspectRatio(float n)
{
    aspect = n;
    dirty_proj = true;
}

void CameraComponent::SetNear(float n)
{
    near = n;
    dirty_proj = true;
}

void CameraComponent::SetFar(float n)
{
    far = n;
    dirty_proj = true;
}

void CameraComponent::UpdateSettings()
{
    if (dirty_proj)
    {
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }
}

glm::mat4 CameraComponent::GetVP()
{
    UpdateSettings();
    return projection * view;
}

void CameraComponent::SetView(const glm::mat4& matrix)
{
    view = matrix;
}