#pragma once
#include "ECS/ECS.hpp"
#include "ECS/Components/Transform.hpp"

struct CameraDestroyedEvent {
    entt::registry* registry;
    entt::entity camera; // The camera that was destroyed
    Transform transform; // Transform properties for access like recent position, rotation, etc

CameraDestroyedEvent(entt::registry* reg, entt::entity cam, Transform trans) : registry(reg), camera(cam), transform(trans) {}
};

struct CameraCreatedEvent {
    entt::registry* registry;
    entt::entity camera; // The camera that was created

    CameraCreatedEvent(entt::registry* reg, entt::entity e) : registry(reg), camera(e) {}
};