#pragma once

#include "math/Matrix4.h"
#include "math/Quaternion.h"

namespace ds_render
{
struct CameraComponent 
{
    // View matrix is just inverse of associated transform matrix
    // ds_math::Matrix4 viewMatrix;
    ds_math::Matrix4 projectionMatrix;
    // Vertical field of view
    float verticalFov;
    // Aspect ratio
    float aspectRatio;
    // Near z clip
    float nearZClip;
    // Far z clip
    float farZClip;
};
}
