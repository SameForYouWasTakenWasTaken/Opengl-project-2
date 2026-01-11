#pragma once
#include "Camera.hpp"

struct CameraComponent {
    Camera* camera;
    Transform transform;
};