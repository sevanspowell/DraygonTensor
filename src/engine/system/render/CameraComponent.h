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
    // Orientation of camera
    ds_math::Quaternion orientation;
};
}
