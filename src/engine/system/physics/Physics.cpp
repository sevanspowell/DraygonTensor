#include <algorithm>
#include <sstream>

#include "engine/system/physics/Physics.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadPhysicsScriptBindings();

extern const char *physicsSystemLuaName;
}

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
    setInertiaTensorCoeffs(mat,
    					   0.3f*mass*(squares.y + squares.z),
    					   0.3f*mass*(squares.x + squares.z),
						   0.3f*mass*(squares.x + squares.y));
}

static void setSphereIntertiaTensor(ds_math::Matrix3 &mat,
									ds_math::scalar radius,
									ds_math::scalar mass) {
	ds_math::scalar inertia = 2/5.0 * mass * radius * radius;
	setInertiaTensorCoeffs(mat, inertia, inertia, inertia);
}

namespace ds
{
// TODO: Update these values for m_physicsWorld constructor
Physics::Physics()
    : m_physicsWorld(0, 0),
      m_fg(ds_phys::Gravity(ds_math::Vector3(0.0f, -1.0f, 0.0f)))
{
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
            	auto nPos = m_transformComponentManager->GetLocalTranslation(transform);
            	auto nOri = m_transformComponentManager->GetLocalOrientation(transform);

            	bool shouldWakeup = false;
            	if (body->getPosition() != nPos) {
                	body->setPosition(nPos);
                	shouldWakeup = true;
            	}

            	if (body->getOrientation() != nOri) {
                	body->setOrientation(nOri);
                	shouldWakeup = true;
            	}

            	if (shouldWakeup && !body->getAwake()) {
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

//static bool loadBoxCollisionPrimitive(ds_phys::RigidBody& rb, float mass, ds_math::Vector3) {

//}

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

            // Create rigid body component
            ds_phys::RigidBody *body = new ds_phys::RigidBody();
            if (isInvMass) {
            	body->setInverseMass(dataInvMass);
            } else {
            	body->setMass(dataMass);
            }

            // Get collision shapes
            std::vector<std::string> collisionShapeKeys =
                componentData.GetObjectKeys("collisionShapes");

            // Create each collision shape
            std::for_each(
                collisionShapeKeys.begin(), collisionShapeKeys.end(),
                [&](const std::string &key)
                {
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
                        // Get shape dimensions
                        std::stringstream colShapeDimKey;
                        std::vector<float> dataDim;

                        colShapeDimKey << colShapeBaseKey.str() << "."
                                       << "dim";

                        if (componentData.GetFloatArray(colShapeDimKey.str(),
                                                        &dataDim))
                        {
                            ds_math::Vector3 dim(dataDim[0], dataDim[1], dataDim[2]);

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

                                if (dataType == "box")
                                {
                                	auto* box = new ds_phys::CollisionBox();
                                	box->halfSize = dim;
                                	box->body = body;
                                	box->offset = ds_math::Matrix4::CreateTranslationMatrix(offset);

                                	body->addCollisionPrimitive(box);

                                	//@todo Per collision shape mass
                                	ds_math::Matrix3 invInertia;
                                	setBlockInertiaTensor(invInertia, dim, body->getMass());
                                	body->setInertiaTensor(invInertia);

                                	m_physicsWorld.addCollisionPrimitive(std::unique_ptr<ds_phys::CollisionPrimitive>(box));

                                } else if (dataType == "sphere") {
                                	auto* sphere = new ds_phys::CollisionSphere();
                                	sphere->radius = dim.x;
                                	sphere->body = body;
                                	sphere->offset = ds_math::Matrix4::CreateTranslationMatrix(offset);

                                	body->addCollisionPrimitive(sphere);

                                	//@todo Per collision shape mass
                                	ds_math::Matrix3 invInertia;
                                	setSphereIntertiaTensor(invInertia, dim.x, body->getMass());
                                	body->setInertiaTensor(invInertia);

                                	m_physicsWorld.addCollisionPrimitive(std::unique_ptr<ds_phys::CollisionPrimitive>(sphere));
                                }
                            }
                        }
                    }

                });



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

            m_physicsWorld.addRigidBody(body);
        }
    }
}
}
