#pragma once

#include "engine/system/ISystem.h"
#include "engine/system/physics/PhysicsComponentManager.h"
#include "engine/system/scene/TransformComponentManager.h"
#include "engine/system/physics/PhysicsWorld.h"

namespace ds
{
class Physics : public ISystem
{
public:
    Physics();

    virtual bool Initialize(const char *configFile);

    virtual void Update(float deltaTime);

    virtual void Shutdown();

    virtual void PostMessages(const ds_msg::MessageStream &messages);

    virtual ds_msg::MessageStream CollectMessages();

    virtual const char *GetName() const;

    virtual ScriptBindingSet GetScriptBindings() const;

    void AddForceGenerator(Entity entity);

    virtual unsigned getUpdateRate(uint32_t screenRefreshRate) const;

    virtual unsigned getMaxConsecutiveUpdates() const;

private:
    void ProcessEvents(ds_msg::MessageStream *messages);

    void CreateTransformComponent(Entity entity, const Config &componentData);

    void CreatePhysicsComponent(Entity entity, const Config &componentData);

    void UpdateRigidBodyTransforms();
    void PropagateTransform();

    /** Messaging */
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    /** Transform component manager */
    TransformComponentManager *m_transformComponentManager;
    /** Physics component manager */
    PhysicsComponentManager *m_physicsComponentManager;

    ds_phys::PhysicsWorld m_physicsWorld;

    // TODO: Temporary!
    ds_phys::Gravity m_fg;
    ds_phys::RigidBody m_body;
};
}
