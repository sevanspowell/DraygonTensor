#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/custom_physics/CustomPhysicsComponent.h"

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
class CustomPhysicsComponentManager
    : public ComponentManager<CustomPhysicsComponent>
{
public:
    /**
     * Get the particle associated with the given component instance.
     *
     * @param   i   Instance, component instance to get the particle of.
     * @return      const ds_phys::Particle &, particle.
     */
    const ds_phys::Particle &GetParticle(Instance i) const;

    /**
     * Associate the given particle with the given component instance.
     *
     * @param   i          Instance, component instance to set particle of.
     * @param   particle   const ds_phys::Particle &, particle to set.
     */
    void SetParticle(Instance i, const ds_phys::Particle &particle);
};
}
