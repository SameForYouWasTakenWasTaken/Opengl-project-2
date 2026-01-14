#pragma once
#include <functional>
#include <any>

#include "ECS/ECS.hpp"

class Event final {
    std::vector<std::any> arguments;
    std::function<void>& callback;
    bool enabled = true;
public:
    template <typename... Args>
    Event(std::function<void(Args...)>& cb);
    
    void On();
    
    // void Disable(entt::dispatcher d, EventType ev);
    // void Enable(entt::dispatcher d, EventType ev);
};