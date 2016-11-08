#pragma once

#include "engine/system/ISystem.h"
#include "engine/system/physics/PhysicsComponentManager.h"
#include "engine/system/physics/PhysicsWorld.h"
#include "engine/system/scene/TransformComponentManager.h"

namespace ds
{
/**
 * The physics system is the "glue" between the physics engine and the game
 * engine.
 */
class Physics : public ISystem
{
public:
    /**
     * Default constructor.
     */
    Physics();

    /**
     * Initialize the physics system.
     *
     * @param   configFile   const char *, NULL-terminated config file string.
     * @return               bool, true if the physics system initialized
     * successfully, false otherwise.
     */
    virtual bool Initialize(const char *configFile);

    /**
     * Update the physics system.
     *
     * @param   deltaTime   float, timestep to update the physics system over.
     */
    virtual void Update(float deltaTime);

    /**
     * Shutdown the physics system, freeing all associated memory.
     */
    virtual void Shutdown();

    /**
     * Post messages to the physics system.
     *
     * @param   messages   const ds_msg::MessageStream &, messages to post to
     * the physics system.
     */
    virtual void PostMessages(const ds_msg::MessageStream &messages);

    /**
     * Receive messages from the physics system.
     *
     * @return   ds_msg::MessageStream, stream of messages returned from the
     * physics system.
     */
    virtual ds_msg::MessageStream CollectMessages();

    /**
     * Get the name of the physics system.
     *
     * @return    const char *, name of the physics system.
     */
    virtual const char *GetName() const;

    /**
     * Get the script bindings.
     *
     * @return   ScriptBindingSet, script binding set returned from physics
     * system.
     */
    virtual ScriptBindingSet GetScriptBindings() const;

    /**
     * Add a force generator to the rigid body associated with an entity.
     *
     * @param   entity           Entity, entity to add force generator to
     * (should have rigid body component - otherwise will silently fail).
     * @param   forceGenerator   std::shared_ptr<ds_phys::IForceGenerator>,
     * force generator to add.
     */
    void
    AddForceGenerator(Entity entity,
                      std::shared_ptr<ds_phys::IForceGenerator> forceGenerator);

    /**
     * Add a collision plane to the physics world.
     *
     * @param   norm     const ds_math::Vector3 &, normal of the plane.
     * @param   offset   ds_math::scalar, offset of the plane.
     * @return           ds_phys::CollisionPrimitiveID, id of the collision
     * primitive created.
     */
    ds_phys::CollisionPrimitiveID addPlane(const ds_math::Vector3 &norm,
                                           ds_math::scalar offset);

    /**
     * Get the update rate of the physics system.
     *
     * @param    screenRefreshRate   uit32_t, screen refresh rate.
     * @return                       unsigned, update rate of the physics
     * system.
     */
    virtual unsigned getUpdateRate(uint32_t screenRefreshRate) const;

    /**
     * Get the maximum number of consecutive updates from the physics system.
     *
     * @return   unsigned, maximum number of consecutive updates from the
     * physics system.
     */
    virtual unsigned getMaxConsecutiveUpdates() const;

    /**
     * Get a pointer to the rigid body associated with given entity.
     *
     * @param   entity   Entity, entity to get rigid body of.
     * @return           ds_phys::RigidBody *, pointer to rigid body associated
     * with the given entity or nullptr if no rigid body associated with that
     * entity.
     */
    ds_phys::RigidBody *getRigidBody(Entity entity);

    /**
     * Set the gravity of the physic simulation.
     *
     * @param   gravity   const ds_math::Vector3 &, gravity vector to use.
     */
    void SetGravity(const ds_math::Vector3 &gravity);

private:
    /**
     * Process the engine events.
     *
     * @param   messages   ds_msg::MessageStream *, engine event stream.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /**
     * Create a transform component from component data.
     *
     * @param   entity          Entity, create a transform component for this
     * entity.
     * @param   componentData   const Config &, component data to create entity
     * from.
     */
    void CreateTransformComponent(Entity entity, const Config &componentData);

    /**
     * Create a physics component from component data.
     *
     * @param   entity          Entity, create a physics component for this
     * entity.
     * @param   componentData   const Config &, component data to create entity
     * from.
     */
    void CreatePhysicsComponent(Entity entity, const char *componentData);

    /**
     * Update the rigid body transforms.
     */
    void UpdateRigidBodyTransforms();
    /**
     * Propogate those transforms.
     */
    void PropagateTransform();

    /** Messaging */
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    /** Transform component manager */
    TransformComponentManager *m_transformComponentManager;
    /** Physics component manager */
    PhysicsComponentManager *m_physicsComponentManager;

    ds_phys::PhysicsWorld m_physicsWorld;

    std::shared_ptr<ds_phys::Gravity> m_gravityFg;
    // ds_phys::ImpulseGenerator *m_impulseFg;
    ds_phys::RigidBody m_body;
};
}
