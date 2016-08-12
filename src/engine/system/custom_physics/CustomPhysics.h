#pragma once

#include "engine/system/ISystem.h"
#include "engine/system/custom_physics/CustomPhysicsComponentManager.h"
#include "engine/system/custom_physics/Particle.h"
#include "engine/system/scene/TransformComponentManager.h"

namespace ds
{
class CustomPhysics : public ISystem
{
public:
    virtual bool Initialize(const Config &config);
    virtual void Update(float deltaTime);
    virtual void Shutdown();
    virtual void PostMessages(const ds_msg::MessageStream &messages);
    virtual ds_msg::MessageStream CollectMessages();

    /**
     * @copydoc ISystem::GetName()
     */
    virtual const char *GetName() const;

    /**
     * Return required script bindings.
     *
     * @return  ScriptBindingSet, the script bindings the physics system wants
     * to register with the Script system.
     */
    virtual ScriptBindingSet GetScriptBindings() const;

    /**
     * Set the mass of the particle associated with the given entity.
     *
     * @param   e      Entity, entity with particle to set mass of.
     * @param   mass   ds_math::scalar, mass of particle to set.
     */
    void SetParticleMass(Entity e, ds_math::scalar mass);

    /**
     * Set the velocity of the particle associated with the given entity.
     *
     * @param   e          Entity, entity with particle to set velocity of.
     * @param   velocity   const ds_math::Vector3 &, velocity of particle to
     * set.
     */
    void SetParticleVelocity(Entity e, const ds_math::Vector3 &velocity);

    /**
     * Set the acceleration of the particle associated with the given entity.
     *
     * @param   e              Entity, entity with particle to set acceleration
     * of.
     * @param   acceleration   const ds_math::Vector3 &, acceleration of
     * particle to set.
     */
    void SetParticleAcceleration(Entity e,
                                 const ds_math::Vector3 &acceleration);

    /**
     * Set the damping of the particle associated with the given entity.
     *
     * @param   e         Entity, entity with particle to set damping of.
     * @param   damping   ds_math::scalar, damping of particle to set.
     */
    void SetParticleDamping(Entity e, ds_math::scalar damping);

private:
    /**
     * Process messages in the given message stream.
     *
     * @param  ds_msg::MessageStream *, messages to process.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /** Transform component manager */
    TransformComponentManager m_transformComponentManager;
    /** Physics component manager */
    CustomPhysicsComponentManager m_physicsComponentManager;

    /** Messaging */
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;
};
}
