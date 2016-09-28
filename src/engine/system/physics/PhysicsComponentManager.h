#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/physics/PhysicsComponent.h"

namespace ds
{
/**
 * Physics component manager.
 *
 * Associates an entity with some physics properties. The memory of each Bullet
 * collisionObject is managed by the Physics system. The Physics system is
 * responsible for deleting PhysicsComponent instances when their rigidbodies
 * are deleted.
 */
class PhysicsComponentManager : public ComponentManager<PhysicsComponent>
{
public:
    ds_phys::RigidBody *GetRigidBody(Instance i);

    void SetRigidBody(Instance i, ds_phys::RigidBody *rigidBody);
};
}
