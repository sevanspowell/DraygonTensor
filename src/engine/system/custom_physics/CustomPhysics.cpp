#include "CustomPhysics.h"

#include "engine/common/StringIntern.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadCustomPhysicsScriptBindings();

extern const char *customPhysicsSystemLuaName;
}

namespace ds
{
bool CustomPhysics::Initialize(const Config &config)
{
    ds_msg::SystemInit initMsg;
    initMsg.systemName = "CustomPhysics";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    return true;
}

void CustomPhysics::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    for (unsigned int i = 0; i < m_physicsComponentManager.GetNumInstances();
         ++i)
    {
        Instance phys = Instance::MakeInstance(i);

        // Get entity for instance
        Entity entity = m_physicsComponentManager.GetEntityForInstance(phys);

        // Check if this entity also has transform component
        Instance transform =
            m_transformComponentManager.GetInstanceForEntity(entity);

        // If it does...
        if (transform.IsValid())
        {
            ds_phys::Particle particle =
                m_physicsComponentManager.GetParticle(phys);

            if (deltaTime > 0)
            {
                particle.integrate(deltaTime);
            }

            // Update local translation
            ds_math::Vector3 currentLocalTranslation =
                m_transformComponentManager.GetLocalTranslation(transform);
            ds_math::Vector3 newLocalTranslation = particle.getPosition();

            ds_msg::SetLocalTranslation setTranslationMsg;
            setTranslationMsg.entity = entity;
            setTranslationMsg.localTranslation = newLocalTranslation;
            ds_msg::AppendMessage(
                &m_messagesGenerated, ds_msg::MessageType::SetLocalTranslation,
                sizeof(ds_msg::SetLocalTranslation), &setTranslationMsg);

            m_physicsComponentManager.SetParticle(phys, particle);
        }
    }

    m_messagesReceived.Clear();
}

void CustomPhysics::Shutdown()
{
}

void CustomPhysics::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messagesReceived, messages);
}

ds_msg::MessageStream CustomPhysics::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

const char *CustomPhysics::GetName() const
{
    return ds_lua::customPhysicsSystemLuaName;
}

ScriptBindingSet CustomPhysics::GetScriptBindings() const
{
    return ds_lua::LoadCustomPhysicsScriptBindings();
}

void CustomPhysics::SetParticleMass(Entity e, ds_math::scalar mass)
{
    ds_msg::SetParticleMass setParticleMassMsg;
    setParticleMassMsg.entity = e;
    setParticleMassMsg.mass = mass;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::SetParticleMass,
                          sizeof(ds_msg::SetParticleMass), &setParticleMassMsg);
}

void CustomPhysics::SetParticleVelocity(Entity e,
                                        const ds_math::Vector3 &velocity)
{
    ds_msg::SetParticleVelocity setParticleVelocityMsg;
    setParticleVelocityMsg.entity = e;
    setParticleVelocityMsg.velocity = velocity;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetParticleVelocity,
        sizeof(ds_msg::SetParticleVelocity), &setParticleVelocityMsg);
}

void CustomPhysics::SetParticleAcceleration(
    Entity e, const ds_math::Vector3 &acceleration)
{
    ds_msg::SetParticleAcceleration setParticleAccelerationMsg;
    setParticleAccelerationMsg.entity = e;
    setParticleAccelerationMsg.acceleration = acceleration;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetParticleAcceleration,
        sizeof(ds_msg::SetParticleAcceleration), &setParticleAccelerationMsg);
}

void CustomPhysics::SetParticleDamping(Entity e, ds_math::scalar damping)
{
    ds_msg::SetParticleDamping setParticleDampingMsg;
    setParticleDampingMsg.entity = e;
    setParticleDampingMsg.damping = damping;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetParticleDamping,
        sizeof(ds_msg::SetParticleDamping), &setParticleDampingMsg);
}

void CustomPhysics::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::CreateComponent:
        {
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Load up component data for component
            Config componentData;
            if (componentData.LoadMemory(StringIntern::Instance().GetString(
                    createComponentMsg.componentData)))
            {
                // Get component type
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
                // Get entity
                Entity entity = createComponentMsg.entity;
                /*
                 * We need both a transform and a physics component to create a
                 * rigidbody, therefore, every time we create either a physics
                 * or transform component, we check if we've already created the
                 * other. If so, we create the rigidbody.
                 */
                // Create transform component
                if (componentType == "transformComponent")
                {
                    // Create transform component
                    Instance transform = TransformComponentManager::
                        CreateComponentForEntityFromConfig(
                            &m_transformComponentManager, entity,
                            componentData);
                    // If this entity also has physics component, create
                    // rigidbody for that physics component
                    Instance phys =
                        m_physicsComponentManager.GetInstanceForEntity(entity);
                    // If physics instance is valid (entity has physics
                    // component too)
                    if (phys.IsValid())
                    {
                        ds_phys::Particle particle =
                            m_physicsComponentManager.GetParticle(phys);

                        ds_math::Vector4 temp =
                            m_transformComponentManager.GetWorldTransform(
                                transform)[3];
                        ds_math::Vector3 origin =
                            ds_math::Vector3(temp.x, temp.y, temp.z);

                        particle.setPosition(origin);

                        m_physicsComponentManager.SetParticle(phys, particle);
                    }
                }
                // Create physics component
                else if (componentType == "physicsComponent")
                {
                    // Create physics component
                    Instance phys =
                        m_physicsComponentManager.CreateComponentForEntity(
                            entity);

                    ds_phys::Particle particle;
                    particle.setDamping(0.995f);
                    particle.setMass(1.0f);

                    ds_math::Vector3 origin =
                        ds_math::Vector3(0.0f, 0.0f, 0.0f);

                    // If this entity also has transform component,
                    // create rigid body at transform location,
                    // else create it at 0, 0, 0
                    Instance transform =
                        m_transformComponentManager.GetInstanceForEntity(
                            entity);
                    if (transform.IsValid())
                    {
                        ds_math::Vector4 temp =
                            m_transformComponentManager.GetWorldTransform(
                                transform)[3];
                        origin = ds_math::Vector3(temp.x, temp.y, temp.z);
                    }

                    particle.setPosition(origin);

                    m_physicsComponentManager.SetParticle(phys, particle);
                }
            }
            break;
        }
        case ds_msg::MessageType::SetLocalTranslation:
        {
            ds_msg::SetLocalTranslation setTranslationMsg;
            (*messages) >> setTranslationMsg;

            // Get component instance of entity to move
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setTranslationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set translation of entity
                m_transformComponentManager.SetLocalTranslation(
                    transform, setTranslationMsg.localTranslation);

                // Get physics component instance of entity to move
                Instance phys = m_physicsComponentManager.GetInstanceForEntity(
                    setTranslationMsg.entity);

                // If has physics component
                if (phys.IsValid())
                {
                    // Get collision object from entity
                    ds_phys::Particle particle =
                        m_physicsComponentManager.GetParticle(phys);

                    ds_math::Vector3 worldTranslation =
                        m_transformComponentManager.GetWorldTranslation(
                            transform);

                    particle.setPosition(worldTranslation);

                    m_physicsComponentManager.SetParticle(phys, particle);
                }
            }

            break;
        }
        case ds_msg::MessageType::DestroyEntity:
        {
            ds_msg::DestroyEntity destroyEntityMsg;
            (*messages) >> destroyEntityMsg;

            Entity e = destroyEntityMsg.entity;

            // Remove entity from all component managers
            // Physics
            // Get component instance for entity
            Instance physics =
                m_physicsComponentManager.GetInstanceForEntity(e);

            // If valid, remove it from physics system
            if (physics.IsValid())
            {
                // Remove from component manager
                m_physicsComponentManager.RemoveInstance(physics);
            }

            // Transform
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(e);

            if (transform.IsValid())
            {
                m_transformComponentManager.RemoveInstance(transform);
            }

            break;
        }
        case ds_msg::MessageType::SetParticleMass:
        {
            ds_msg::SetParticleMass setParticleMassMsg;
            (*messages) >> setParticleMassMsg;

            Entity e = setParticleMassMsg.entity;

            // Get component instance for entity
            Instance phys = m_physicsComponentManager.GetInstanceForEntity(e);

            if (phys.IsValid())
            {
                ds_phys::Particle particle =
                    m_physicsComponentManager.GetParticle(phys);
                particle.setMass(setParticleMassMsg.mass);
                m_physicsComponentManager.SetParticle(phys, particle);
            }

            break;
        }
        case ds_msg::MessageType::SetParticleVelocity:
        {
            ds_msg::SetParticleVelocity setParticleVelocityMsg;
            (*messages) >> setParticleVelocityMsg;

            Entity e = setParticleVelocityMsg.entity;

            // Get component instance for entity
            Instance phys = m_physicsComponentManager.GetInstanceForEntity(e);

            if (phys.IsValid())
            {
                ds_phys::Particle particle =
                    m_physicsComponentManager.GetParticle(phys);
                particle.setVelocity(setParticleVelocityMsg.velocity);
                m_physicsComponentManager.SetParticle(phys, particle);
            }

            break;
        }
        case ds_msg::MessageType::SetParticleAcceleration:
        {
            ds_msg::SetParticleAcceleration setParticleAccelerationMsg;
            (*messages) >> setParticleAccelerationMsg;

            Entity e = setParticleAccelerationMsg.entity;

            // Get component instance for entity
            Instance phys = m_physicsComponentManager.GetInstanceForEntity(e);

            if (phys.IsValid())
            {
                std::cout << "Reached" << std::endl;
                ds_phys::Particle particle =
                    m_physicsComponentManager.GetParticle(phys);
                particle.setAcceleration(
                    setParticleAccelerationMsg.acceleration);
                m_physicsComponentManager.SetParticle(phys, particle);
            }

            break;
        }
        case ds_msg::MessageType::SetParticleDamping:
        {
            ds_msg::SetParticleDamping setParticleDampingMsg;
            (*messages) >> setParticleDampingMsg;

            Entity e = setParticleDampingMsg.entity;

            // Get component instance for entity
            Instance phys = m_physicsComponentManager.GetInstanceForEntity(e);

            if (phys.IsValid())
            {
                ds_phys::Particle particle =
                    m_physicsComponentManager.GetParticle(phys);
                particle.setDamping(setParticleDampingMsg.damping);
                m_physicsComponentManager.SetParticle(phys, particle);
            }

            break;
        }
        default:
            messages->Extract(header.size);
            break;
        }
    }
}
}
