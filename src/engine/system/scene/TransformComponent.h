#pragma once

#include "engine/entity/Instance.h"
#include "math/Matrix4.h"

namespace ds
{
struct TransformComponent
{
    ds_math::Matrix4 localTransform;
    ds_math::Matrix4 worldTransform;
    Instance parent;
    Instance firstChild;
    Instance nextSibling;
    Instance prevSibling;
};
}
