#include "engine/common/StringIntern.h"
#include "engine/message/MessageHelper.h"
#include "engine/system/physics/Physics.h"

namespace ds
{
bool Physics::Initialize(const Config &config)
{
    // Collision configuration contains default setup for memory and collision.
    // Advanced users can create their own configurations.
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

    // Use the default collision dispatcher. For parallel processing you can use
    // a different dispatcher.
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

    // btDbvtBroadphase is a good general-purpose broadphase. You can also try
    // out btAxis3Sweep.
    m_overlappingPairCache = new btDbvtBroadphase();

    // The default constraint solver. For parallel-processing you can use a
    // different solver.
    m_solver = new btSequentialImpulseConstraintSolver();

    m_dynamicsWorld =
        new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache,
                                    m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    // // Add a few rigid bodies
    // btCollisionShape *groundShape = new btBoxShape(
    //     btVector3(btScalar(50.0), btScalar(50.0), btScalar(50.0)));

    // m_collisionShapes.push_back(groundShape);

    // btTransform groundTransform;
    // groundTransform.setIdentity();
    // groundTransform.setOrigin(btVector3(0, -56, 0));

    // {
    //     btScalar mass(0.0);

    //     // Rigidbody is dynamic if and only if mass is non-zero, otherwise
    //     // static
    //     bool isDynamic = (mass != 0.0f);

    //     btVector3 localInertia(0, 0, 0);
    //     if (isDynamic)
    //     {
    //         groundShape->calculateLocalInertia(mass, localInertia);
    //     }

    //     // Using motion state is optional (provides interpolation
    //     capabilities
    //     // and
    //     // only synchronizes 'active' objects)
    //     btDefaultMotionState *myMotionState =
    //         new btDefaultMotionState(groundTransform);
    //     btRigidBody::btRigidBodyConstructionInfo rbInfo(
    //         mass, myMotionState, groundShape, localInertia);
    //     btRigidBody *body = new btRigidBody(rbInfo);

    //     // Add the body to the dynamics world
    //     m_dynamicsWorld->addRigidBody(body);
    // }

    // // Create a dynamic rigidbody
    // {
    //     btCollisionShape *colShape = new btSphereShape(btScalar(1.0));
    //     m_collisionShapes.push_back(colShape);

    //     // Create dynamic objects
    //     btTransform startTransform;
    //     startTransform.setIdentity();

    //     btScalar mass(1.0f);

    //     // Rigid body is dynamic if and only if ;mass is non-zero, otherwise
    //     // static
    //     bool isDynamic = (mass != 0.0f);

    //     btVector3 localInertia(0, 0, 0);
    //     if (isDynamic)
    //     {
    //         colShape->calculateLocalInertia(mass, localInertia);

    //         startTransform.setOrigin(btVector3(2, 10, 0));

    //         // Using motion state is recommended.
    //         btDefaultMotionState *myMotionState =
    //             new btDefaultMotionState(startTransform);
    //         btRigidBody::btRigidBodyConstructionInfo rbInfo(
    //             mass, myMotionState, colShape, localInertia);
    //         btRigidBody *body = new btRigidBody(rbInfo);

    //         m_dynamicsWorld->addRigidBody(body);
    //     }
    // }

    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Physics";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    return true;
}

void Physics::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    // Move simulation forward
    m_dynamicsWorld->stepSimulation(deltaTime, 10);

    // Iterate thru physics components
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
            // Get rigid body from physics component
            const btRigidBody *rigidBody =
                m_physicsComponentManager.GetRigidBody(phys);

            // Get new transform
            btTransform bulletNewWorldTransform;
            if (rigidBody != nullptr)
            {
                if (rigidBody->getMotionState())
                {
                    rigidBody->getMotionState()->getWorldTransform(
                        bulletNewWorldTransform);
                }
                else
                {
                    bulletNewWorldTransform = rigidBody->getWorldTransform();
                }

                // Get new world transform in our math library
                const ds_math::Matrix4 newWorldTransform;
                bulletNewWorldTransform.getOpenGLMatrix(
                    (btScalar *)&newWorldTransform.data[0][0]);

                // Get current transforms
                const ds_math::Matrix4 &currentWorldTransform =
                    m_transformComponentManager.GetWorldTransform(transform);
                const ds_math::Matrix4 &currentLocalTransform =
                    m_transformComponentManager.GetLocalTransform(transform);

                // New local transform is current local transform multiplied by
                // (inv(currentWorldTransform) * newWorldTransform * currentLocalTransform)
                ds_math::Matrix4 newLocalTransform =
                    ds_math::Matrix4::Inverse(currentWorldTransform) *
                    newWorldTransform * currentLocalTransform;

                ds_msg::SetLocalTransform setTransformMsg;
                setTransformMsg.entity = entity;
                setTransformMsg.localTransform = newLocalTransform;

                ds_msg::AppendMessage(&m_messagesGenerated,
                                      ds_msg::MessageType::SetLocalTransform,
                                      sizeof(ds_msg::SetLocalTransform),
                                      &setTransformMsg);

                std::cout << "Object: " << i << " Pos: "
                          << (float)bulletNewWorldTransform.getOrigin().getX()
                          << ", "
                          << (float)bulletNewWorldTransform.getOrigin().getY()
                          << ", "
                          << (float)bulletNewWorldTransform.getOrigin().getZ()
                          << std::endl;
            }
        }
    }

    // Print positions of all objects
    // for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; --j)
    // {
    //     btCollisionObject *obj =
    //     m_dynamicsWorld->getCollisionObjectArray()[j];
    //     btRigidBody *body = btRigidBody::upcast(obj);
    //     btTransform trans;
    //     if (body && body->getMotionState())
    //     {
    //         body->getMotionState()->getWorldTransform(trans);
    //     }
    //     else
    //     {
    //         trans = obj->getWorldTransform();
    //     }

    //     std::cout << "Object: " << j
    //               << " Pos: " << (float)trans.getOrigin().getX() << ", "
    //               << (float)trans.getOrigin().getY() << ", "
    //               << (float)trans.getOrigin().getZ() << std::endl;
    // }
}

void Physics::Shutdown()
{
    // Delete Bullet objects in reverse order of creation
    // Remove rigidbodies from dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i)
    {
        btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // Delete collision shapes
    for (int j = 0; j < m_collisionShapes.size(); ++j)
    {
        btCollisionShape *shape = m_collisionShapes[j];
        m_collisionShapes[j] = 0;

        delete shape;
    }

    // Delete dynamics world;
    delete m_dynamicsWorld;

    // Delete solver
    delete m_solver;

    // Delete broadphase
    delete m_overlappingPairCache;

    // Delete dispatcher
    delete m_dispatcher;
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

void Physics::ProcessEvents(ds_msg::MessageStream *messages)
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
                std::cout << "Reached: " << createComponentMsg.componentType
                          << std::endl;
                // Get component type
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
                std::cout << "Physics: component created " << componentType
                          << std::endl;
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
                    // If physics instance is valid (entit has physics component
                    // too)
                    if (phys.IsValid())
                    {
                        StringIntern::StringId shape =
                            m_physicsComponentManager.GetShape(phys);
                        ds_math::Vector3 scale =
                            m_physicsComponentManager.GetScale(phys);
                        float mass = m_physicsComponentManager.GetMass(phys);
                        ds_math::Vector4 temp =
                            m_transformComponentManager.GetLocalTransform(
                                transform)[3];
                        ds_math::Vector3 origin =
                            ds_math::Vector3(temp.x, temp.y, temp.z);
                        // Create rigidbody
                        btRigidBody *rigidBody =
                            CreateRigidBody(origin, shape, scale, mass);

                        // Add to physics component
                        m_physicsComponentManager.SetRigidBody(phys, rigidBody);

                        // Add rigibody to world
                        m_dynamicsWorld->addRigidBody(rigidBody);
                    }
                }
                // Create physics component
                if (componentType == "physicsComponent")
                {
                    // Get physics component data.
                    std::string dataShape;
                    std::vector<float> dataScale;
                    float dataMass;

                    // If successfully got all data
                    if (componentData.GetString("shape", &dataShape) &&
                        componentData.GetFloatArray("scale", &dataScale) &&
                        componentData.GetFloat("mass", &dataMass))
                    {
                        // Ensure float array is correct size
                        if (dataScale.size() == 3)
                        {
                            // Create physics component
                            Instance phys =
                                m_physicsComponentManager
                                    .CreateComponentForEntity(entity);
                            m_physicsComponentManager.SetRigidBody(phys,
                                                                   nullptr);
                            m_physicsComponentManager.SetShape(
                                phys,
                                StringIntern::Instance().Intern(dataShape));
                            m_physicsComponentManager.SetScale(
                                phys,
                                ds_math::Vector3(dataScale[0], dataScale[1],
                                                 dataScale[2]));
                            m_physicsComponentManager.SetMass(phys, dataMass);

                            // If this entity also has transform component,
                            // create rigidbody for this physics component
                            Instance transform =
                                m_transformComponentManager
                                    .GetInstanceForEntity(entity);
                            // If transform instance is valid (entity has
                            // transform component too)
                            if (transform.IsValid())
                            {
                                StringIntern::StringId shape =
                                    m_physicsComponentManager.GetShape(phys);
                                ds_math::Vector3 scale =
                                    m_physicsComponentManager.GetScale(phys);
                                float mass =
                                    m_physicsComponentManager.GetMass(phys);
                                ds_math::Vector4 temp =
                                    m_transformComponentManager
                                        .GetLocalTransform(transform)[3];
                                ds_math::Vector3 origin =
                                    ds_math::Vector3(temp.x, temp.y, temp.z);
                                // Create rigidbody
                                btRigidBody *rigidBody =
                                    CreateRigidBody(origin, shape, scale, mass);

                                if (rigidBody != nullptr)
                                {
                                    // Add to physics component
                                    m_physicsComponentManager.SetRigidBody(
                                        phys, rigidBody);

                                    // Add rigibody to world
                                    m_dynamicsWorld->addRigidBody(rigidBody);
                                }
                            }
                        }
                        else
                        {
                            std::cout << "Physics component not created, scale "
                                         "vector size not equal to 3"
                                      << std::endl;
                        }
                    }
                }
            }
        }
        break;
        case ds_msg::MessageType::SetLocalTransform:
        {
            ds_msg::SetLocalTransform setLocalMsg;
            (*messages) >> setLocalMsg;

            // Get component instance of entity to move
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setLocalMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set transform of entity
                m_transformComponentManager.SetLocalTransform(
                    transform, setLocalMsg.localTransform);
            }
        }
        break;
        default:
            messages->Extract(header.size);
            break;
        }
    }
}

btRigidBody *Physics::CreateRigidBody(const ds_math::Vector3 &origin,
                                      StringIntern::StringId shape,
                                      const ds_math::Vector3 &scale,
                                      float mass)
{
    btRigidBody *rigidBody = nullptr;

    // Create transform
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

    // Create collision shape
    btCollisionShape *colShape = nullptr;
    std::string colShapeType = StringIntern::Instance().GetString(shape);
    std::cout << "Collision shape type: " << colShapeType << std::endl;
    if (colShapeType == "box")
    {
        colShape = new btBoxShape(
            btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z)));
        m_collisionShapes.push_back(colShape);
    }
    else if (colShapeType == "sphere")
    {
        colShape = new btSphereShape(btScalar(scale.x));
        m_collisionShapes.push_back(colShape);
    }

    if (colShape != nullptr)
    {
        // Create rigid body
        btScalar bodyMass = btScalar(mass);

        // Rigid body is dynamic if and only if mass is non-zero, otherwise
        // static
        bool isDynamic = (bodyMass != 0.0f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
        {
            colShape->calculateLocalInertia(mass, localInertia);
        }

        btDefaultMotionState *bodyMotionState =
            new btDefaultMotionState(bodyTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(
            bodyMass, bodyMotionState, colShape, localInertia);

        std::cout << "Rigid body created " << mass << std::endl;
        rigidBody = new btRigidBody(rbInfo);
    }
    else
    {
        std::cerr
            << "Physics::CreateRigidBody: Unhandled collision shape type: "
            << colShapeType << std::endl;
    }

    return rigidBody;
}
}
