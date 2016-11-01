#include <algorithm>
#include <sstream>

#include "engine/json/Json.h"
#include "engine/message/MessageHelper.h"
#include "engine/system/physics/Physics.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadPhysicsScriptBindings();

extern const char *physicsSystemLuaName;
}
/*
static void setInertiaTensorCoeffs(ds_math::Matrix3 &mat,
                                   ds_math::scalar ix,
                                   ds_math::scalar iy,
                                   ds_math::scalar iz,
                                   ds_math::scalar ixy = 0,
                                   ds_math::scalar ixz = 0,
                                   ds_math::scalar iyz = 0)
{
    mat[0][0] = ix;
    mat[0][1] = -ixy;
    mat[0][2] = -ixz;
    mat[1][0] = -ixy;
    mat[1][1] = iy;
    mat[1][2] = -iyz;
    mat[2][0] = -ixz;
    mat[2][1] = -iyz;
    mat[2][2] = iz;
}

static void setBlockInertiaTensor(ds_math::Matrix3 &mat,
                                  const ds_math::Vector3 &halfSizes,
                                  ds_math::scalar mass)
{
    ds_math::Vector3 squares = halfSizes * halfSizes;
    setInertiaTensorCoeffs(mat, 0.3f * mass * (squares.y + squares.z),
                           0.3f * mass * (squares.x + squares.z),
                           0.3f * mass * (squares.x + squares.y));
}


// static void setSphereIntertiaTensor(ds_math::Matrix3 &mat,
//									ds_math::scalar radius,
//									ds_math::scalar mass) {
//	ds_math::scalar inertia = 2/5.0 * mass * radius * radius;
//	setInertiaTensorCoeffs(mat, inertia, inertia, inertia);
//}
static void setSphereIntertiaTensor(ds_math::Matrix3 &mat,
                                    ds_math::scalar radius,
                                    ds_math::scalar mass)
{
    ds_math::scalar inertia = 2 / 5.0 * mass * radius * radius;
    setInertiaTensorCoeffs(mat, inertia, inertia, inertia);
}
*/
namespace ds
{
// TODO: Update these values for m_physicsWorld constructor
Physics::Physics()
    : m_physicsWorld(0, 0),
      m_fg(ds_phys::Gravity(ds_math::Vector3(0.0f, -10.0f, 0.0f)))
{
    // addPlane(ds_math::Vector3(0, 1, 0), 0);
}

bool Physics::Initialize(const char *configFile)
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

        m_physicsWorld.addForceGenerator(body, &m_fg);
    }
}

ds_phys::CollisionPrimitiveID Physics::addPlane(const ds_math::Vector3 &norm,
                                                ds_math::scalar offset)
{
    ds_phys::CollisionPlane *plane = new ds_phys::CollisionPlane();
    plane->direction = ds_math::Vector3::Normalize(norm);
    plane->offset = offset;
    return m_physicsWorld.addCollisionPrimitive(
        std::unique_ptr<ds_phys::CollisionPrimitive>(plane));
}

unsigned Physics::getUpdateRate(uint32_t screenRefreshRate) const
{
    return screenRefreshRate * 2;
}

unsigned Physics::getMaxConsecutiveUpdates() const
{
    return 1;
}

void Physics::Update(float deltaTime)
{

    UpdateRigidBodyTransforms();

    ProcessEvents(&m_messagesReceived);

    m_physicsWorld.startFrame();

    m_physicsWorld.stepSimulation(deltaTime);

    // std::cout << m_physicsWorld.m_rigidBodies[0]->getPosition() << std::endl;
    PropagateTransform();

    m_messagesReceived.Clear();
}

void Physics::UpdateRigidBodyTransforms()
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
                auto nPos =
                    m_transformComponentManager->GetLocalTranslation(transform);
                auto nOri =
                    m_transformComponentManager->GetLocalOrientation(transform);

                bool shouldWakeup = false;
                if (body->getPosition() != nPos)
                {
                    body->setPosition(nPos);
                    shouldWakeup = true;
                }

                if (body->getOrientation() != nOri)
                {
                    body->setOrientation(nOri);
                    shouldWakeup = true;
                }

                if (shouldWakeup && !body->getAwake())
                {
                    body->setAwake(true);
                }
            }
        }
    }
}

void Physics::PropagateTransform()
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
                    CreatePhysicsComponent(
                        entity, StringIntern::Instance()
                                    .GetString(createComponentMsg.componentData)
                                    .c_str());
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

// static bool loadBoxCollisionPrimitive(ds_phys::RigidBody& rb, float mass,
// ds_math::Vector3) {

//}

void Physics::CreatePhysicsComponent(Entity entity, const char *componentData)
{
    std::cout << "Adding physics component for: " << entity.id << std::endl;
    // std::cout << "data: " << componentData << std::endl;
    if (componentData == nullptr)
    {
        return;
    }

    // Does entity already have physics component?
    Instance phys = m_physicsComponentManager->GetInstanceForEntity(entity);

    // If not, create one
    if (!phys.IsValid())
    {
        JsonObject root;
        json::parseObject(componentData, &root);

        // Get physics component data.
        // These are default values.
        float dataRestitution = 0.6f;
        float dataDamping = 0.3f;
        float dataAngularDamping = 0.21f;

        if (root["restitution"] != nullptr)
        {
            dataRestitution = json::parseFloat(root["restitution"]);
        }
        if (root["damping"] != nullptr)
        {
            dataDamping = json::parseFloat(root["damping"]);
        }
        if (root["angularDamping"] != nullptr)
        {
            dataAngularDamping = json::parseFloat(root["angularDamping"]);
        }


        // Create rigid body component
        ds_phys::RigidBody *body = new ds_phys::RigidBody();

        std::vector<float> invMasses;
        std::vector<ds_math::Vector3> offsets;
        std::vector<ds_math::Vector3> inverseInertiaTensors;

        if (root["collisionShapes"] != nullptr)
        {
            JsonArray collisionShapes;
            json::parseArray(root["collisionShapes"], &collisionShapes);

            for (unsigned int i = 0; i < collisionShapes.size(); ++i)
            {
                JsonObject collisionShape;
                json::parseObject(collisionShapes[i], &collisionShape);

                // Get collision shape name
                std::string name;
                if (collisionShape["name"] != nullptr)
                {
                    json::parseString(collisionShape["name"], &name);
                }
                else
                {
                    std::cerr << "Collision shape missing name." << std::endl;
                    continue;
                }

                // Only get more data if we have each of these fields, they are
                // necessary for calculating rigidbody info
                if ((collisionShape["mass"] == nullptr &&
                     collisionShape["invMass"] == nullptr) ||
                    collisionShape["offset"] == nullptr ||
                    (collisionShape["inertiaTensor"] == nullptr &&
                     collisionShape["invInertiaTensor"] == nullptr))
                {
                    std::cerr << "Collision shape missing one or more of "
                                 "mass/invMass, offset and "
                                 "inertiaTensor/invInertiaTensor fields."
                              << std::endl;
                    continue;
                }

                // Get collision shape mass
                if (collisionShape["mass"] != nullptr)
                {
                    float mass = json::parseFloat(collisionShape["mass"]);
                    assert(mass != 0.0f);
                    mass = (mass == 0.0f ? 1.0f : mass);

                    invMasses.push_back(1.0f / mass);
                }
                else
                {
                    invMasses.push_back(
                        json::parseFloat(collisionShape["invMass"]));
                }

                // Get collision shape offset
                JsonArray offset;
                json::parseArray(collisionShape["offset"], &offset);

                ds_math::Vector3 tmpOffset;
                assert(offset.size() == 3);
                // Take first 3 values of array
                for (unsigned int j = 0; j < 3; ++j)
                {
                    tmpOffset[j] = json::parseFloat(offset[j]);
                }
                offsets.push_back(tmpOffset);

                // Get inertia tensor
                if (collisionShape["inertiaTensor"] != nullptr)
                {
                    JsonArray inertiaT;
                    json::parseArray(collisionShape["inertiaTensor"],
                                     &inertiaT);

                    assert(inertiaT.size() == 3);
                    ds_math::Vector3 tmpInertiaTensor;
                    // Take first 3 values of array
                    for (unsigned int j = 0; j < 3; ++j)
                    {
                        float inertiaProduct = json::parseFloat(inertiaT[j]);
                        assert(inertiaProduct != 0);
                        tmpInertiaTensor[j] = 1.0f / inertiaProduct;
                    }
                    inverseInertiaTensors.push_back(tmpInertiaTensor);
                }
                else if (collisionShape["invInertiaTensor"] != nullptr)
                {
                    JsonArray invInertiaT;
                    json::parseArray(collisionShape["invInertiaTensor"],
                                     &invInertiaT);

                    assert(invInertiaT.size() == 3);
                    ds_math::Vector3 tmpInverseInertiaTensor;
                    // Take first 3 values of array
                    for (unsigned int j = 0; j < 3; ++j)
                    {
                        tmpInverseInertiaTensor[j] =
                            json::parseFloat(invInertiaT[j]);
                    }
                    inverseInertiaTensors.push_back(tmpInverseInertiaTensor);
                }

                std::string type;
                if (collisionShape["type"] != nullptr)
                {
                    json::parseString(collisionShape["type"], &type);
                }
                else
                {
                    std::cerr << "Collision shape " << i << " (" << name
                              << ") needs type field." << std::endl;
                    continue;
                }

                if (type == "box")
                {
                    ds_math::Vector3 dimensions;
                    ds_math::Vector3 offset;
                    ds_math::Vector3 inertiaTensor;

                    if (collisionShape["halfSize"] != nullptr)
                    {
                        JsonArray dim;
                        json::parseArray(collisionShape["halfSize"], &dim);

                        assert(dim.size() == 3);
                        // Take first 3 values of array
                        for (unsigned int j = 0; j < 3; ++j)
                        {
                            dimensions[j] = json::parseFloat(dim[j]);
                        }
                    }
                    else
                    {
                        std::cerr << "Collision shape " << i << " (" << name
                                  << ") needs halfSize field." << std::endl;
                        continue;
                    }


                    // Create box
                    auto *box = new ds_phys::CollisionBox();
                    box->halfSize = dimensions;
                    box->body = body;
                    box->offset =
                        ds_math::Matrix4::CreateTranslationMatrix(offsets[i]);

                    body->addCollisionPrimitive(box);

                    m_physicsWorld.addCollisionPrimitive(
                        std::unique_ptr<ds_phys::CollisionPrimitive>(box));
                }
                else if (type == "sphere")
                {
                    float radius = 0.0f;

                    if (collisionShape["radius"] != nullptr)
                    {
                        radius = json::parseFloat(collisionShape["radius"]);
                    }
                    else
                    {
                        std::cerr << "Collision shape " << i << " (" << name
                                  << ") needs radius field." << std::endl;
                        continue;
                    }

                    auto *sphere = new ds_phys::CollisionSphere();
                    sphere->radius = radius;
                    sphere->body = body;
                    sphere->offset =
                        ds_math::Matrix4::CreateTranslationMatrix(offsets[i]);

                    body->addCollisionPrimitive(sphere);

                    m_physicsWorld.addCollisionPrimitive(
                        std::unique_ptr<ds_phys::CollisionPrimitive>(sphere));
                }
                else if (type == "capsule")
                {
                    float radius = 0.0f;
                    float height = 0.0f;

                    if (collisionShape["radius"] != nullptr)
                    {
                        radius = json::parseFloat(collisionShape["radius"]);
                    }
                    else
                    {
                        std::cerr << "Collision shape " << i << " (" << name
                                  << ") needs radius field." << std::endl;
                    }
                    if (collisionShape["height"] != nullptr)
                    {
                        height = json::parseFloat(collisionShape["height"]);
                    }
                    else
                    {
                        std::cerr << "Collision shape " << i << " (" << name
                                  << ") needs height field." << std::endl;
                        continue;
                    }

                    std::cout << "radius: " << radius << " height: " << height
                              << std::endl;
                    auto *capsule = new ds_phys::CollisionCapsule();
                    capsule->radius = radius;
                    capsule->height = height;
                    capsule->offset =
                        ds_math::Matrix4::CreateTranslationMatrix(offsets[i]);

                    body->addCollisionPrimitive(capsule);

                    m_physicsWorld.addCollisionPrimitive(
                        std::unique_ptr<ds_phys::CollisionPrimitive>(capsule));
                }
                else
                {
                    std::cerr << "Collisions shape " << i << " (" << name
                              << ") has unknown type field." << std::endl;
                }
            }

            phys = m_physicsComponentManager->CreateComponentForEntity(entity);
            m_physicsComponentManager->SetRigidBody(phys, body);

            // If this entity also has a transform component, update rigidbody
            // with
            // pos, orientation and scale of that transform component
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

            // Sum masses
            float totalMass = 0.0f;
            for (unsigned int i = 0; i < invMasses.size(); ++i)
            {
                if (invMasses[i] == 0.0f)
                {
                    totalMass = -1.0f;
                    break;
                }

                totalMass += 1.0f / invMasses[i];
            }

            if (totalMass < 0.0f)
            {
                body->setInverseMass(0.0f);
                body->setCenterOfMassLocalSpace(
                    ds_math::Vector3(0.0f, 0.0f, 0.0f));
                // No need for inertia tensor, immovable object
            }
            else
            {
                body->setMass(totalMass);

                assert(invMasses.size() == offsets.size() &&
                       invMasses.size() == inverseInertiaTensors.size());
                const size_t numColShapes = invMasses.size();

                // Calculate composite inertia tensor
                // Calculate centre of mass relative to body
                ds_math::Vector3 compositeInertiaTensor(0.0f, 0.0f, 0.0f);
                ds_math::Vector3 centerOfMass(0.0f, 0.0f, 0.0f);
                for (unsigned int i = 0; i < numColShapes; ++i)
                {
                    assert(invMasses[i] != 0);
                    centerOfMass += ((1.0f / invMasses[i]) * offsets[i]);
                }

                for (unsigned int i = 0; i < numColShapes; ++i)
                {
                    // Calculate pos of each collision shape relative to center
                    // of mass
                    ds_math::Vector3 comOffset = -centerOfMass + offsets[i];

                    // Use parallel axis theorum to transform current inertia
                    // tensor from center of mass of collision shape to center
                    // of mass of rigid body
                    ds_math::Vector3 inertiaTensor;
                    for (unsigned int j = 0; j < 3; ++j)
                    {
                        // Get inertia tensor from inverse inertia tensor
                        if (inverseInertiaTensors[i][j] != 0)
                        {
                            assert(inverseInertiaTensors[i][j] != 0);
                            inertiaTensor[j] =
                                1.0f / inverseInertiaTensors[i][j];
                        }

                        // Calculate new inertia tensor for this shape
                        assert(invMasses[i] != 0);
                        inertiaTensor[j] +=
                            (1.0f / invMasses[i]) * pow(comOffset[j], 2);
                    }

                    // Sum new inertia tensors to form composite inertia tensor
                    compositeInertiaTensor += inertiaTensor;
                }

                body->setInertiaTensor(compositeInertiaTensor);
                body->setCenterOfMassLocalSpace(centerOfMass);
            }

            // body->setRestitution(dataRestitution);
            body->setLinearDamping(dataDamping);
            body->setAngularDamping(dataAngularDamping);

            m_physicsWorld.addForceGenerator(body, &m_fg);
            m_physicsWorld.addRigidBody(body);
        }
    }
}
}
