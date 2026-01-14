#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

struct Transform {
    glm::vec3 position{0.f};
    glm::vec3 rotation{0.f};
    glm::vec3 scale{1.f};

    // Incremental functions, basically increments current state by the given one
    void Move(const glm::vec3& increment);
    void Scale(const glm::vec3& increment);
    void Rotate(const glm::vec3& degrees);

    // Setter-functions. Sets whatever value you need it to be
    void SetPosition(const glm::vec3& new_pos);
    void SetScale(const glm::vec3& new_scale);
    void SetRotation(const glm::vec3& new_rotation);
    
    // These are just standalone calculations. TRhese dont change the private variables at all.
    glm::vec3 LookAtVec3(const glm::vec3& target);
};