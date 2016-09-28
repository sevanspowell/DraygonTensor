#pragma once

#include "math/Vector3.h"

#include "engine/system/physics/RigidBody.h"

namespace ds
{
/**
 * The PhysicsComponent holds information about the physics system properties of
 * an entity.
 */
struct PhysicsComponent
{
    ds_phys::RigidBody *rigidBody;
};
}
