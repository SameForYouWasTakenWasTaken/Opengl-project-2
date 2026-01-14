#pragma once
#include <memory>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "ECS/Components/Transform.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Events/CameraEvents.hpp"


// TODO: Make in a separate file ✅
class CameraSystem {
    entt::dispatcher& dispatcher;
public:
    entt::entity active_cam_entity;

    CameraSystem(entt::registry& reg, entt::dispatcher& dispatcher);

    void OnCameraDestroyed(entt::registry& reg, entt::entity entity);
    void OnCameraCreated(entt::registry& reg, entt::entity entity);

    void RenderSystem(entt::registry& reg);
    void UpdateSystem(entt::registry& reg, float dt);
};