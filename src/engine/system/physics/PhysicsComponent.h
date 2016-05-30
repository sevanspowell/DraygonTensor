#pragma once

#include "btBulletDynamicsCommon.h"

#include "engine/common/StringIntern.h"
#include "math/Vector3.h"

namespace ds
{
/**
 * The PhysicsComponent holds information about the physics system properties of
 * an entity.
 */
struct PhysicsComponent
{
    // Collision shape type
    StringIntern::StringId shape;
    // Shape scaling
    ds_math::Vector3 scale;
    // Mass of rigid body
    float mass;
    // Bullet collision object
    btCollisionObject *collisionObject;
    // Bullet collision shape
    btCollisionShape *collisionShape;
};
}
