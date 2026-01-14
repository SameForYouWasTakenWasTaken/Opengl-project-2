#include "App/CameraSystem.hpp"

CameraSystem::CameraSystem(entt::registry& reg, entt::dispatcher& dispatcher)
: dispatcher(dispatcher)
{
    reg.on_destroy<CameraComponent>().connect<&CameraSystem::OnCameraDestroyed>(*this);
    reg.on_construct<CameraComponent>().connect<&CameraSystem::OnCameraCreated>(*this);
}

void CameraSystem::OnCameraDestroyed(entt::registry& reg, entt::entity e)
{

    // Check if it has the transform
    if (!reg.all_of<Transform>(e))
    {
        spdlog::error("OnCameraDestroyed() returned an error! : No transform property in camera!");    
        return;
    }

    auto trans = reg.get<Transform>(e); // Copy instead of reference (intentional)

    dispatcher.trigger(CameraDestroyedEvent{
        &reg, e, trans
    });
    
    spdlog::info("Deleted a camera!");
}

void CameraSystem::OnCameraCreated(entt::registry& reg, entt::entity cam)
{

    // Check if it has the transform
    if (!reg.all_of<Transform>(cam))
    {
        spdlog::error("OnCameraCreated() returned an error! : No transform property in camera!");    
        return;
    }
        
    dispatcher.trigger<CameraCreatedEvent>(CameraCreatedEvent{
        &reg, cam
    });
    
    spdlog::info("Created a camera!");
}

void CameraSystem::RenderSystem(entt::registry& reg)
{
    auto cameras = reg.view<CameraComponent, Transform>();

    cameras.each([this, &reg](auto entity, CameraComponent& cam, Transform& transform){
        if (!reg.valid(active_cam_entity))
        {
            // If this specific entity is valid
            if (reg.valid(entity))
            {
                active_cam_entity = entity; // Set the first valid camera entity to be active
                return;
            }
        }

        return; // No point, we already have an active camera entity
    });

    // Check again
    if (!reg.valid(active_cam_entity)) 
    {
        spdlog::warn("CameraSystem::RenderSystem() returned a warning! : No active camera!");
        return;
    }

    auto& cam = reg.get<CameraComponent>(active_cam_entity);
    auto& trans = reg.get<Transform>(active_cam_entity);

    cam.SetView(glm::lookAt(trans.position, trans.position + cam.cam_front, cam.cam_up));
}

void CameraSystem::UpdateSystem(entt::registry& reg, float dt)
{

}