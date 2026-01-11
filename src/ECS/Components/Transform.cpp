#include "ECS/Components/Transform.hpp"

void Transform::Move(const glm::vec3& increment)
{
    position += increment;
}

void Transform::Rotate(const glm::vec3& increment)
{
    rotation += increment;
    rotation = glm::mod(rotation, glm::vec3(360.f));
}

void Transform::Scale(const glm::vec3& increment)
{
    scale += increment;
}

void Transform::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

void Transform::SetRotation(const glm::vec3& rot)
{
    rotation = rot;
    rotation = glm::mod(rotation, glm::vec3(360.f));
}

void Transform::SetScale(const glm::vec3& _scale)
{
    scale = _scale;
}