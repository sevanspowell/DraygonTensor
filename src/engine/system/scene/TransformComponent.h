#pragma once

#include "engine/entity/Instance.h"
#include "math/Matrix4.h"

namespace ds
{
struct TransformComponent
{
    ds_math::Vector3 localTranslation;
    ds_math::Vector3 localScale;
    ds_math::Quaternion localOrientation;

    // Cache world data
    ds_math::Vector3 worldTranslation;
    ds_math::Vector3 worldScale;
    ds_math::Quaternion worldOrientation;

    Instance parent;
    Instance firstChild;
    Instance nextSibling;
    Instance prevSibling;
};
}

// TODO add methods for manipulating
