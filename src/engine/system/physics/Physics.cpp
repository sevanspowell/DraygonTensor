#include <algorithm>
#include <sstream>

#include "engine/system/physics/Physics.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadPhysicsScriptBindings();

extern const char *physicsSystemLuaName;
}

namespace ds
{
// TODO: Update these values for m_physicsWorld constructor
Physics::Physics()
    : m_physicsWorld(0, 0),
      m_fg(ds_phys::Gravity(ds_math::Vector3(0.0f, -1.0f, 0.0f)))
{
}

bool Physics::Initialize(const Config &config)
{
    m_transformComponentManager =
        GetComponentStore().GetComponentManager<TransformComponentManager>();
    m_physicsComponentManager =
        GetComponentStore().GetComponentManager<PhysicsComponentManager>();

    return true;
}

void Physics::AddForceGenerator(Entity entity)
{
    Instance phys = m_physicsComponentManager->GetInstanceForEntity(entity);

    if (phys.IsValid())
    {
        ds_phys::RigidBody *body =
            m_physicsComponentManager->GetRigidBody(phys);

        assert(body != nullptr);

        // m_physicsWorld.addForceGenerator(body, &m_fg);
    }
}

unsigned Physics::getUpdateRate(uint32_t screenRefreshRate) const {
	return screenRefreshRate * 2;
}

unsigned Physics::getMaxConsecutiveUpdates() const {
	return 1;
}

void Physics::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    m_physicsWorld.startFrame();

    m_physicsWorld.stepSimulation(deltaTime);

    // std::cout << m_physicsWorld.m_rigidBodies[0]->getPosition() << std::endl;
    UpdateComponents();

    m_messagesReceived.Clear();
}

void Physics::UpdateComponents()
{
    // Loop thru everything with a physics rigid body
    for (unsigned int i = 0; i < m_physicsComponentManager->GetNumInstances();
         ++i)
    {
        Instance phys = Instance::MakeInstance(i);

        // Get entity for instance
        Entity entity = m_physicsComponentManager->GetEntityForInstance(phys);

        // Get rigidbody
        ds_phys::RigidBody *body =
            m_physicsComponentManager->GetRigidBody(phys);

        assert(body != nullptr);

        {
            Instance transform =
                m_transformComponentManager->GetInstanceForEntity(entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set translation of entity
                m_transformComponentManager->SetLocalTranslation(
                    transform, body->getPosition());

                // Set orientation of entity
                m_transformComponentManager->SetLocalOrientation(
                    transform, body->getOrientation());
            }
        }
    }
}

void Physics::Shutdown()
{
    // TODO: loop thru each physics component and free rigid bodies
}

void Physics::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messagesReceived, messages);
}

ds_msg::MessageStream Physics::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

const char *Physics::GetName() const
{
    return ds_lua::physicsSystemLuaName;
}

ScriptBindingSet Physics::GetScriptBindings() const
{
    return ds_lua::LoadPhysicsScriptBindings();
}

void Physics::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
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

                // Create transform component
                if (componentType == "transformComponent")
                {
                    CreateTransformComponent(entity, componentData);
                }
                // Create physics component
                else if (componentType == "physicsComponent")
                {
                    CreatePhysicsComponent(entity, componentData);
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
                m_physicsComponentManager->GetInstanceForEntity(e);

            // If valid, remove it from component manager
            if (physics.IsValid())
            {
                m_physicsComponentManager->RemoveInstance(physics);
            }

            // Transform
            Instance transform =
                m_transformComponentManager->GetInstanceForEntity(e);

            if (transform.IsValid())
            {
                m_transformComponentManager->RemoveInstance(transform);
            }

            break;
        }
        // case ds_msg::MessageType::SetLocalTranslation:
        // {
        //     ds_msg::SetLocalTranslation setTranslationMsg;
        //     (*messages) >> setTranslationMsg;

        //     // Get component instance of entity to move
        //     Instance transform =
        //         m_transformComponentManager->GetInstanceForEntity(
        //             setTranslationMsg.entity);

        //     // If has transform component
        //     if (transform.IsValid())
        //     {
        //         // Set translation of entity
        //         m_transformComponentManager->SetLocalTranslation(
        //             transform, setTranslationMsg.localTranslation);
        //     }

        //     break;
        // }
        // case ds_msg::MessageType::SetLocalOrientation:
        // {
        //     ds_msg::SetLocalOrientation setLocalOrientation;
        //     (*messages) >> setLocalOrientation;

        //     // Get component instance of entity to move
        //     Instance transform =
        //         m_transformComponentManager->GetInstanceForEntity(
        //             setLocalOrientation.entity);

        //     // If has transform component
        //     if (transform.IsValid())
        //     {
        //         // Set translation of entity
        //         m_transformComponentManager->SetLocalOrientation(
        //             transform, setLocalOrientation.localOrientation);
        //     }

        //     break;
        // }
        default:
        {
            // Extract message to prevent corrupting message stream
            messages->Extract(header.size);
            break;
        }
        }
    }
}

void Physics::CreateTransformComponent(Entity entity,
                                       const Config &componentData)
{
    // Does entity already have transform component?
    Instance transform =
        m_transformComponentManager->GetInstanceForEntity(entity);

    // If not, create one
    if (!transform.IsValid())
    {
        // Create transform component
        transform =
            TransformComponentManager::CreateComponentForEntityFromConfig(
                m_transformComponentManager, entity, componentData);

        // If this entity also has physics component, update it with new
        // pos,
        // orientation, scale, etc.
        Instance phys = m_physicsComponentManager->GetInstanceForEntity(entity);
        // If physics instance is valid (entity has phys component too)
        if (phys.IsValid())
        {
            ds_phys::RigidBody *rigidBody =
                m_physicsComponentManager->GetRigidBody(phys);

            if (rigidBody != nullptr)
            {
                ds_math::Vector4 temp =
                    m_transformComponentManager->GetWorldTransform(
                        transform)[3];
                ds_math::Vector3 origin =
                    ds_math::Vector3(temp.x, temp.y, temp.z);

                // Update position
                rigidBody->setPosition(origin);
            }
        }
    }
}

void Physics::CreatePhysicsComponent(Entity entity, const Config &componentData)
{
    // Does entity already have physics component?
    Instance phys = m_physicsComponentManager->GetInstanceForEntity(entity);

    // If not, create one
    if (!phys.IsValid())
    {
        // Get physics component data.
        float dataMass;
        float dataInvMass;
        float dataDamping;
        float dataAngularDamping;
        float dataRestitution;
        std::vector<float> dataInvInertiaTensor;
        std::vector<float> dataInertiaTensor;

        // Get data that can only be in one form
        if (componentData.GetFloat("restitution", &dataRestitution) &&
            componentData.GetFloat("angularDamping", &dataAngularDamping) &&
            componentData.GetFloat("damping", &dataDamping))
        {
            bool isInvMass = false;
            bool isInvInertiaTensor = false;

            ds_math::Vector3 invInertiaTensor;
            ds_math::Vector3 inertiaTensor;

            if (componentData.GetFloat("mass", &dataMass))
            {
                isInvMass = false;
            }
            else if (componentData.GetFloat("invMass", &dataInvMass))
            {
                isInvMass = true;
            }
            else
            {
                // Make sure we get atleast one piece of mass data
                return;
            }

            if (componentData.GetFloatArray("inertiaTensor",
                                            &dataInertiaTensor))
            {
                isInvInertiaTensor = false;
                inertiaTensor =
                    ds_math::Vector3(dataInertiaTensor[0], dataInertiaTensor[1],
                                     dataInertiaTensor[2]);
            }
            else if (componentData.GetFloatArray("invInertiaTensor",
                                                 &dataInvInertiaTensor))
            {
                isInvInertiaTensor = true;
                invInertiaTensor = ds_math::Vector3(dataInvInertiaTensor[0],
                                                    dataInvInertiaTensor[1],
                                                    dataInvInertiaTensor[2]);
            }
            else
            {
                // Make sure we get atleast one piece of inertia
                // tensor data
                return;
            }

            std::cout << std::endl
                      << " -- Prefab component contents -- " << std::endl;
            std::cout << "restitution: " << dataRestitution << std::endl;
            std::cout << "damping: " << dataDamping << std::endl;
            std::cout << "angularDamping: " << dataAngularDamping << std::endl;
            if (isInvMass == false)
            {
                std::cout << "mass: " << dataMass << std::endl;
            }
            else
            {
                std::cout << "inv. mass: " << dataInvMass << std::endl;
            }
            if (isInvInertiaTensor == false)
            {
                std::cout << "inertia tensor: " << inertiaTensor << std::endl;
            }
            else
            {
                std::cout << "inv. inertia tensor: " << invInertiaTensor
                          << std::endl;
            }


            // Get collision shapes
            std::vector<std::string> collisionShapeKeys =
                componentData.GetObjectKeys("collisionShapes");

            std::cout << "collisionShapes:" << std::endl;
            // Create each collision shape
            std::for_each(
                collisionShapeKeys.begin(), collisionShapeKeys.end(),
                [&](const std::string &key)
                {
                    std::cout << "\t" << key << ": ";

                    // Get key path to collision shape
                    std::stringstream colShapeBaseKey;
                    colShapeBaseKey << "collisionShapes"
                                    << "." << key;

                    // Get shape type
                    std::stringstream colShapeTypeKey;
                    std::string dataType;

                    colShapeTypeKey << colShapeBaseKey.str() << "."
                                    << "type";

                    if (componentData.GetString(colShapeTypeKey.str(),
                                                &dataType))
                    {
                        std::cout << "type: " << dataType << ", ";

                        // Get shape dimensions
                        std::stringstream colShapeDimKey;
                        std::vector<float> dataDim;

                        colShapeDimKey << colShapeBaseKey.str() << "."
                                       << "dim";

                        if (componentData.GetFloatArray(colShapeDimKey.str(),
                                                        &dataDim))
                        {
                            ds_math::Vector3 dim(dataDim[0], dataDim[1],
                                                 dataDim[2]);
                            std::cout << "dim: " << dim << ", ";

                            // Get shape offset
                            std::stringstream colShapeOffsetKey;
                            std::vector<float> dataOffset;

                            colShapeOffsetKey << colShapeBaseKey.str() << "."
                                              << "offset";

                            if (componentData.GetFloatArray(
                                    colShapeOffsetKey.str(), &dataOffset))
                            {
                                ds_math::Vector3 offset(dataOffset[0],
                                                        dataOffset[1],
                                                        dataOffset[2]);

                                std::cout << "offset: " << offset << std::endl;

                                if (dataType == "box")
                                {
                                    // ds_phys::CollisionBox
                                }
                            }
                        }
                    }

                });

            // Create rigid body component
            ds_phys::RigidBody *body = new ds_phys::RigidBody();

            body->setMass(1.0f);

            // DEBUG
            static int tmpValue = 0;
            if (tmpValue == 0)
            {
                m_physicsWorld.m_box.body = body;
                tmpValue++;
                std::cout << "Obj1" << std::endl;
            }
            else if (tmpValue == 1)
            {
                m_physicsWorld.m_box2.body = body;
                tmpValue++;
                std::cout << "Obj2" << std::endl;
            }


            phys = m_physicsComponentManager->CreateComponentForEntity(entity);
            m_physicsComponentManager->SetRigidBody(phys, body);

            // TODO: Set mass etc.
            // If this entity also has a transform component, update
            // rigidbody
            // with pos, orientation and scale of that transform component
            Instance transform =
                m_transformComponentManager->GetInstanceForEntity(entity);

            if (transform.IsValid())
            {

                ds_math::Vector4 temp =
                    m_transformComponentManager->GetWorldTransform(
                        transform)[3];
                ds_math::Vector3 origin =
                    ds_math::Vector3(temp.x, temp.y, temp.z);

                body->setPosition(origin);
                body->setOrientation(
                    m_transformComponentManager->GetWorldOrientation(
                        transform));
            }

            // Add rigid body component to world
            // body->setVelocity(ds_math::Vector3(0, 0, 0));
            // body->setRotation(ds_math::Vector3(0, 0, 0));
            // ds_math::scalar mass = 0.5f * 0.5f * 0.5f * 8.0f;
            // body->setMass(mass);

            // ds_math::Matrix3 tensor;
            // tensor.set;
            // body->setInertiaTensor(tensor);

            // body->setLinearDamping(0.95f);
            // body->setAngularDamping(0.8f);
            // body->clearAccumulators();
            // body->setAcceleration(0, -10.0f, 0.0f);
            // body->calculateDerivedData();
            m_physicsWorld.addRigidBody(body);
        }
    }
}
}
