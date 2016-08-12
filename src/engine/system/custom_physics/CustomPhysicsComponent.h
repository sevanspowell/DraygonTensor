#pragma once

#include "engine/common/StringIntern.h"
#include "engine/system/custom_physics/Particle.h"
#include "math/Vector3.h"

namespace ds
{
/**
 * The CustomPhysicsComponent holds information about the physics system
 * properties of an entity.
 */
struct CustomPhysicsComponent
{
    ds_phys::Particle particle;
};
}
