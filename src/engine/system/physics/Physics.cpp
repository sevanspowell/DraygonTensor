#include <sstream>

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "engine/common/StringIntern.h"
#include "engine/message/MessageHelper.h"
#include "engine/resource/TextureResource.h"
#include "engine/system/physics/Physics.h"
#include "engine/system/render/RenderCommon.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadPhysicsScriptBindings();
}

namespace ds
{
bool Physics::Initialize(const Config &config)
{
    // Register factory creators
    m_factory.RegisterCreator<TextureResource>(TextureResource::CreateFromFile);
    m_factory.RegisterCreator<TerrainResource>(TerrainResource::CreateFromFile);

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

    // Create dynamics world
    m_dynamicsWorld =
        new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache,
                                    m_solver, m_collisionConfiguration);

    // Set gravity
    m_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

    // Enable use of ghost objects
    m_dynamicsWorld->getBroadphase()
        ->getOverlappingPairCache()
        ->setInternalGhostPairCallback(new btGhostPairCallback());

    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Physics";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    m_isPaused = false;

    return true;
}

void Physics::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    // Only update if not paused
    if (m_isPaused == false)
    {
        // Move simulation forward
        m_dynamicsWorld->stepSimulation(deltaTime, 10);

        // Iterate thru physics components
        for (unsigned int i = 0;
             i < m_physicsComponentManager.GetNumInstances(); ++i)
        {
            Instance phys = Instance::MakeInstance(i);

            // Get entity for instance
            Entity entity =
                m_physicsComponentManager.GetEntityForInstance(phys);

            // Check if this entity also has transform component
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(entity);

            // If it does...
            if (transform.IsValid())
            {
                // Get collision object
                const btCollisionObject *colObject =
                    m_physicsComponentManager.GetCollisionObject(phys);

                if (colObject != nullptr)
                {
                    if (colObject->getInternalType() ==
                        btCollisionObject::CollisionObjectTypes::CO_RIGID_BODY)
                    {
                        // Downcast collision object to rigidbody
                        const btRigidBody *rigidBody =
                            (const btRigidBody *)colObject;

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
                                bulletNewWorldTransform =
                                    rigidBody->getWorldTransform();
                            }

                            // Get change in translation
                            ds_math::Matrix4 newWorldTransform;
                            bulletNewWorldTransform.getOpenGLMatrix(
                                (btScalar *)&newWorldTransform.data[0][0]);
                            ds_math::Vector3 newWorldTranslation =
                                ds_math::Vector3(newWorldTransform[3].x,
                                                 newWorldTransform[3].y,
                                                 newWorldTransform[3].z);
                            ds_math::Vector3 oldWorldTranslation =
                                m_transformComponentManager.GetWorldTranslation(
                                    transform);
                            ds_math::Vector3 changeInTranslation =
                                newWorldTranslation - oldWorldTranslation;
                            // Change local translation by change in translation
                            ds_math::Vector3 currentLocalTranslation =
                                m_transformComponentManager.GetLocalTranslation(
                                    transform);
                            ds_msg::SetLocalTranslation setTranslationMsg;
                            setTranslationMsg.entity = entity;
                            setTranslationMsg.localTranslation =
                                currentLocalTranslation + changeInTranslation;
                            ds_msg::AppendMessage(
                                &m_messagesGenerated,
                                ds_msg::MessageType::SetLocalTranslation,
                                sizeof(ds_msg::SetLocalTranslation),
                                &setTranslationMsg);

                            // Get change in orientation
                            btQuaternion newWorldOrientationBullet =
                                rigidBody->getOrientation();
                            btQuaternionFloatData floatData;
                            newWorldOrientationBullet.serializeFloat(floatData);
                            ds_math::Quaternion newWorldOrientation =
                                ds_math::Quaternion(floatData.m_floats[0],
                                                    floatData.m_floats[1],
                                                    floatData.m_floats[2],
                                                    floatData.m_floats[3]);
                            ds_math::Quaternion oldWorldOrientation =
                                ds_math::Quaternion::Normalize(
                                    m_transformComponentManager
                                        .GetWorldOrientation(transform));
                            ds_math::Quaternion changeInOrientation =
                                ds_math::Quaternion::Normalize(
                                    newWorldOrientation *
                                    ds_math::Quaternion::Invert(
                                        oldWorldOrientation));
                            // Change local orientation by change in orientation
                            ds_math::Quaternion currentLocalOrientation =
                                m_transformComponentManager.GetLocalOrientation(
                                    transform);
                            ds_msg::SetLocalOrientation setOrientationMsg;
                            setOrientationMsg.entity = entity;
                            setOrientationMsg.localOrientation =
                                ds_math::Quaternion::Normalize(
                                    changeInOrientation *
                                    currentLocalOrientation);
                            ds_msg::AppendMessage(
                                &m_messagesGenerated,
                                ds_msg::MessageType::SetLocalOrientation,
                                sizeof(ds_msg::SetLocalOrientation),
                                &setOrientationMsg);

                            // Get change in scale
                            btVector3 newWorldScaleBullet =
                                rigidBody->getCollisionShape()
                                    ->getLocalScaling();
                            ds_math::Vector3 newWorldScale =
                                ds_math::Vector3(newWorldScaleBullet.getX(),
                                                 newWorldScaleBullet.getY(),
                                                 newWorldScaleBullet.getZ());
                            ds_math::Vector3 oldWorldScale =
                                m_transformComponentManager.GetWorldScale(
                                    transform);
                            ds_math::Vector3 changeInScale =
                                newWorldScale *
                                ds_math::Vector3(1.0f / oldWorldScale.x,
                                                 1.0f / oldWorldScale.y,
                                                 1.0f / oldWorldScale.z);
                            // Change local scale by change in scale
                            ds_math::Vector3 currentLocalScale =
                                m_transformComponentManager.GetLocalScale(
                                    transform);
                            ds_msg::SetLocalScale setScaleMsg;
                            setScaleMsg.entity = entity;
                            setScaleMsg.localScale =
                                currentLocalScale * changeInScale;
                            ds_msg::AppendMessage(
                                &m_messagesGenerated,
                                ds_msg::MessageType::SetLocalScale,
                                sizeof(ds_msg::SetLocalScale), &setScaleMsg);
                        }

                        // Iterate thru contact manifolds
                        int numManifolds =
                            m_dynamicsWorld->getDispatcher()->getNumManifolds();
                        for (unsigned int j = 0; j < numManifolds; ++j)
                        {
                            // Get colliding bodies
                            btPersistentManifold *contactManifold =
                                m_dynamicsWorld->getDispatcher()
                                    ->getManifoldByIndexInternal(j);
                            const btCollisionObject *objA =
                                contactManifold->getBody0();
                            const btCollisionObject *objB =
                                contactManifold->getBody1();

                            // Get component instance for collision objects
                            Instance physA =
                                m_physicsComponentManager
                                    .GetInstanceForCollisionObject(objA);
                            Instance physB =
                                m_physicsComponentManager
                                    .GetInstanceForCollisionObject(objB);
                            // Find entity for component instance
                            Entity a =
                                m_physicsComponentManager.GetEntityForInstance(
                                    physA);
                            Entity b =
                                m_physicsComponentManager.GetEntityForInstance(
                                    physB);

                            // Iterate thru contact points
                            int numContacts = contactManifold->getNumContacts();
                            for (int k = 0; k < numContacts; ++k)
                            {
                                // Get collision info
                                btManifoldPoint &pt =
                                    contactManifold->getContactPoint(k);
                                if (pt.getDistance() < 0.0f)
                                {
                                    const btVector3 &ptA =
                                        pt.getPositionWorldOnA();
                                    const btVector3 &ptB =
                                        pt.getPositionWorldOnB();
                                    const btVector3 &normalOnB =
                                        pt.m_normalWorldOnB;

                                    // Create collision message
                                    ds_msg::PhysicsCollision collisionMsg;
                                    collisionMsg.entityA = a;
                                    collisionMsg.entityB = b;
                                    collisionMsg.pointWorldOnA =
                                        ds_math::Vector3(ptA.getX(), ptA.getY(),
                                                         ptA.getZ());
                                    collisionMsg.pointWorldOnB =
                                        ds_math::Vector3(ptB.getX(), ptB.getY(),
                                                         ptB.getZ());
                                    collisionMsg.normalWorldOnB =
                                        ds_math::Vector3(normalOnB.getX(),
                                                         normalOnB.getY(),
                                                         normalOnB.getZ());

                                    // Send it
                                    ds_msg::AppendMessage(
                                        &m_messagesGenerated,
                                        ds_msg::MessageType::PhysicsCollision,
                                        sizeof(ds_msg::PhysicsCollision),
                                        &collisionMsg);
                                }
                            }
                        }
                    }
                    else if (colObject->getInternalType() ==
                             btCollisionObject::CollisionObjectTypes::
                                 CO_GHOST_OBJECT)
                    {
                        // Downcast collision object to ghost object
                        btPairCachingGhostObject *ghostObject =
                            (btPairCachingGhostObject *)colObject;

                        std::cout << "We have a ghost object." << std::endl;

                        btManifoldArray manifoldArray;
                        btBroadphasePairArray &pairArray =
                            ghostObject->getOverlappingPairCache()
                                ->getOverlappingPairArray();
                        int numPairs = pairArray.size();
                        std::cout << "num pairs: " << numPairs << std::endl;

                        for (int j = 0; j < numPairs; ++j)
                        {
                            manifoldArray.clear();

                            const btBroadphasePair &pair = pairArray[j];

                            btBroadphasePair *collisionPair =
                                m_dynamicsWorld->getPairCache()->findPair(
                                    pair.m_pProxy0, pair.m_pProxy1);

                            if (!collisionPair)
                            {
                                std::cout << "No collision pair" << std::endl;
                                continue;
                            }

                            if (collisionPair->m_algorithm)
                            {
                                std::cout << "Algorithm" << std::endl;
                                collisionPair->m_algorithm
                                    ->getAllContactManifolds(manifoldArray);
                            }

                            for (int k = 0; k < manifoldArray.size(); ++k)
                            {
                                std::cout << "Inner manifold" << std::endl;
                                btPersistentManifold *manifold =
                                    manifoldArray[k];

                                // bool isFirstBody =
                                //     (manifold->getBody0() == ghostObject);

                                // btScalar direction =
                                //     isFirstBody ? btScalar(-1.0) :
                                //     btScalar(1.0);

                                for (int p = 0; p < manifold->getNumContacts();
                                     ++p)
                                {
                                    const btManifoldPoint &pt =
                                        manifold->getContactPoint(p);

                                    if (pt.getDistance() < 0.0f)
                                    {
                                        std::cout << "Nyan" << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Iterate thru manifolds
        int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
        for (unsigned int i = 0; i < numManifolds; ++i)
        {
            // Get colliding bodies
            btPersistentManifold *contactManifold =
                m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject *objA = contactManifold->getBody0();
            const btCollisionObject *objB = contactManifold->getBody1();

            // Get component instance for collision objects
            Instance physA =
                m_physicsComponentManager.GetInstanceForCollisionObject(
                    (btRigidBody *)objA);
            Instance physB =
                m_physicsComponentManager.GetInstanceForCollisionObject(
                    (btRigidBody *)objB);
            // Find entity for component instance
            Entity a = m_physicsComponentManager.GetEntityForInstance(physA);
            Entity b = m_physicsComponentManager.GetEntityForInstance(physB);

            // Iterate thru contact points
            int numContacts = contactManifold->getNumContacts();
            for (int j = 0; j < numContacts; ++j)
            {
                // Get collision info
                btManifoldPoint &pt = contactManifold->getContactPoint(j);
                if (pt.getDistance() < 0.0f)
                {
                    const btVector3 &ptA = pt.getPositionWorldOnA();
                    const btVector3 &ptB = pt.getPositionWorldOnB();
                    const btVector3 &normalOnB = pt.m_normalWorldOnB;

                    // Create collision message
                    ds_msg::PhysicsCollision collisionMsg;
                    collisionMsg.entityA = a;
                    collisionMsg.entityB = b;
                    collisionMsg.pointWorldOnA =
                        ds_math::Vector3(ptA.getX(), ptA.getY(), ptA.getZ());
                    collisionMsg.pointWorldOnB =
                        ds_math::Vector3(ptB.getX(), ptB.getY(), ptB.getZ());
                    collisionMsg.normalWorldOnB = ds_math::Vector3(
                        normalOnB.getX(), normalOnB.getY(), normalOnB.getZ());

                    // Send it
                    ds_msg::AppendMessage(&m_messagesGenerated,
                                          ds_msg::MessageType::PhysicsCollision,
                                          sizeof(ds_msg::PhysicsCollision),
                                          &collisionMsg);
                }
            }
        }
    }
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

ScriptBindingSet Physics::GetScriptBindings() const
{
    return ds_lua::LoadPhysicsScriptBindings();
}

float Physics::GetTerrainHeight(Entity entity, const ds_math::Vector3 &position)
{
    float result = 0.0f;

    btVector3 start = btVector3(position.x, position.y, position.z);
    btVector3 end = btVector3(position.x, position.y - 1000.0f, position.z);

    btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

    // Perform raycast
    m_dynamicsWorld->rayTest(start, end, rayCallback);

    if (rayCallback.hasHit())
    {
        end = rayCallback.m_hitPointWorld;
        btVector3 normal = rayCallback.m_hitNormalWorld;

        btVector3 startToEnd = start - end;
        result = ds_math::Vector3::Magnitude(ds_math::Vector3(
            startToEnd.getX(), startToEnd.getY(), startToEnd.getZ()));
    }

    return result;
}

Physics::Raycast Physics::PerformRaycast(const ds_math::Vector3 &rayStart,
                                         const ds_math::Vector3 &rayEnd)
{
    Raycast result = {rayStart, rayEnd, ds_math::Vector3(0.0f, 0.0f, 0.0f),
                      false};

    btVector3 start = btVector3(rayStart.x, rayStart.y, rayStart.z);
    btVector3 end = btVector3(rayEnd.x, rayEnd.y, rayEnd.z);

    btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

    // Perform raycast
    m_dynamicsWorld->rayTest(start, end, rayCallback);

    if (rayCallback.hasHit())
    {
        btVector3 normal = rayCallback.m_hitNormalWorld;

        result.start = rayStart;
        result.end.x = rayCallback.m_hitPointWorld.getX();
        result.end.y = rayCallback.m_hitPointWorld.getY();
        result.end.z = rayCallback.m_hitPointWorld.getZ();
        result.normal =
            ds_math::Vector3(normal.getX(), normal.getY(), normal.getZ());
        result.hasHit = true;
    }

    return result;
}

void Physics::SetLinearVelocity(Entity entity, const ds_math::Vector3 &velocity)
{
    // Get physics component of entity to set linear velocity of.
    Instance phys = m_physicsComponentManager.GetInstanceForEntity(entity);

    // If we have one
    if (phys.IsValid())
    {
        // Get the rigid body for this message
        btRigidBody *rigidBody =
            (btRigidBody *)m_physicsComponentManager.GetCollisionObject(phys);

        if (rigidBody != nullptr)
        {
            // btTransform worldTransform;
            // rigidBody->getMotionState()->getWorldTransform(worldTransform);

            rigidBody->setLinearVelocity(
                btVector3(velocity.x, velocity.y, velocity.z));

            // rigidBody->getMotionState()->setWorldTransform(worldTransform);
            // rigidBody->setCenterOfMassTransform(worldTransform);
        }
    }
}

ds_math::Vector3 Physics::GetLinearVelocity(Entity entity) const
{
    ds_math::Vector3 linearVelocity = ds_math::Vector3(0.0f, 0.0f, 0.0f);

    // Get physics component of entity to set linear velocity of.
    Instance phys = m_physicsComponentManager.GetInstanceForEntity(entity);

    // If we have one
    if (phys.IsValid())
    {
        // Get the rigid body for this message
        btRigidBody *rigidBody =
            (btRigidBody *)m_physicsComponentManager.GetCollisionObject(phys);

        if (rigidBody != nullptr)
        {
            btVector3 currentLinearVelocity = rigidBody->getLinearVelocity();
            linearVelocity = ds_math::Vector3(currentLinearVelocity.getX(),
                                              currentLinearVelocity.getY(),
                                              currentLinearVelocity.getZ());
        }
    }

    return linearVelocity;
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
                        btCollisionObject *colObject =
                            m_physicsComponentManager.GetCollisionObject(phys);

                        if (colObject != nullptr)
                        {
                            if (colObject->getInternalType() ==
                                btCollisionObject::CollisionObjectTypes::
                                    CO_RIGID_BODY)
                            {
                                btRigidBody *rigidBody =
                                    (btRigidBody *)colObject;

                                if (rigidBody != nullptr)
                                {
                                    ds_math::Vector4 temp =
                                        m_transformComponentManager
                                            .GetWorldTransform(transform)[3];
                                    ds_math::Vector3 origin = ds_math::Vector3(
                                        temp.x, temp.y, temp.z);

                                    btTransform worldTransform;
                                    worldTransform.setIdentity();
                                    worldTransform.setOrigin(btVector3(
                                        origin.x, origin.y, origin.z));

                                    // Delete old motion state
                                    delete rigidBody->getMotionState();

                                    // Create new motion state
                                    btDefaultMotionState *bodyMotionState =
                                        new btDefaultMotionState(
                                            worldTransform);
                                    rigidBody->setMotionState(bodyMotionState);
                                }
                            }
                        }
                    }
                }
                // Create physics component
                else if (componentType == "physicsComponent")
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
                            m_physicsComponentManager.SetCollisionObject(
                                phys, nullptr);
                            m_physicsComponentManager.SetShape(
                                phys,
                                StringIntern::Instance().Intern(dataShape));
                            m_physicsComponentManager.SetScale(
                                phys,
                                ds_math::Vector3(dataScale[0], dataScale[1],
                                                 dataScale[2]));
                            m_physicsComponentManager.SetMass(phys, dataMass);

                            StringIntern::StringId shape =
                                m_physicsComponentManager.GetShape(phys);
                            ds_math::Vector3 scale =
                                m_physicsComponentManager.GetScale(phys);
                            float mass =
                                m_physicsComponentManager.GetMass(phys);

                            ds_math::Vector3 origin =
                                ds_math::Vector3(0.0f, 0.0f, 0.0f);

                            // If this entity also has transform component,
                            // create rigid body at transform location,
                            // else create it at 0, 0, 0
                            Instance transform =
                                m_transformComponentManager
                                    .GetInstanceForEntity(entity);
                            if (transform.IsValid())
                            {
                                ds_math::Vector4 temp =
                                    m_transformComponentManager
                                        .GetWorldTransform(transform)[3];
                                origin =
                                    ds_math::Vector3(temp.x, temp.y, temp.z);
                            }

                            // If shape is not a ghost shape ('ghost' prefix)
                            if (dataShape.find("ghost") != 0)
                            {
                                // Create rigid body and add to component
                                // managers
                                CreateRigidBody(phys, origin, shape, scale,
                                                mass);
                            }
                            else
                            {
                                // Create ghost object
                                CreateGhostObject(phys, origin, shape, scale);
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
                else if (componentType == "terrainComponent")
                {
                    std::string heightmapPath;
                    float heightScale;

                    if (componentData.GetString("heightmap", &heightmapPath) &&
                        componentData.GetFloat("heightScale", &heightScale))
                    {
                        // Construct full heightmap path
                        std::stringstream fullPath;
                        fullPath << "../assets/" << heightmapPath;

                        // Create physics component
                        Instance phys =
                            m_physicsComponentManager.CreateComponentForEntity(
                                entity);

                        // Height map shape
                        m_physicsComponentManager.SetShape(
                            phys, StringIntern::Instance().Intern("heightmap"));

                        ds_math::Vector3 origin =
                            ds_math::Vector3(0.0f, 0.0f, 0.0f);

                        // If this entity also has a transform component, create
                        // rigid body at that transform. Otherwise create at 0,
                        // 0, 0
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

                        // Get the terrain data
                        TerrainResource *terrainResource =
                            m_factory.CreateResource<TerrainResource>(
                                         fullPath.str())
                                .release();

                        if (terrainResource != nullptr)
                        {
                            // Create rigid body
                            CreateHeightMapRigidBody(
                                phys, origin, terrainResource, heightScale);

                            // If successful, store terrain resource, get
                            // handle to it
                            ds_render::TerrainResourceHandle
                                terrainResourceHandle =
                                    (ds_render::TerrainResourceHandle)
                                        m_handleManager.Add(
                                            (void *)terrainResource, 0);

                            // Associate terrain resource with entity
                            Instance terrain =
                                m_terrainComponentManager
                                    .CreateComponentForEntity(entity);
                            m_terrainComponentManager.SetTerrainResourceHandle(
                                terrain, terrainResourceHandle);
                        }
                        else
                        {
                            std::cerr << "Physics::CreateHeightmapRigidBody: "
                                         "failed to load heightmap: "
                                      << fullPath.str() << "." << std::endl;
                        }
                    }
                }
                else if (componentType == "characterComponent")
                {
                    Instance phys =
                        m_physicsComponentManager.CreateComponentForEntity(
                            entity);
                    m_physicsComponentManager.SetCollisionObject(phys, nullptr);
                    m_physicsComponentManager.SetShape(
                        phys, StringIntern::Instance().Intern("capsule"));
                    m_physicsComponentManager.SetScale(
                        phys, ds_math::Vector3(0.0f, 0.0f, 0.0f));
                    m_physicsComponentManager.SetMass(phys, 3.0f);

                    ds_math::Vector3 origin =
                        ds_math::Vector3(0.0f, 0.0f, 0.0f);

                    // If this entity also has the transform component, create
                    // rigid body at transform location, else create it at 0, 0,
                    // 0
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

                    // Create rigid body
                    CreatePlayerCapsule(phys, origin, 0.5f, 1.2f, 3.0f);
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
                    btCollisionObject *collisionObject =
                        m_physicsComponentManager.GetCollisionObject(phys);

                    if (collisionObject != nullptr)
                    {
                        btTransform t;

                        t = collisionObject->getWorldTransform();

                        ds_math::Vector3 worldTranslation =
                            m_transformComponentManager.GetWorldTranslation(
                                transform);

                        t.setOrigin(btVector3(worldTranslation.x,
                                              worldTranslation.y,
                                              worldTranslation.z));

                        collisionObject->setWorldTransform(t);
                    }
                }
            }

            break;
        }
        case ds_msg::MessageType::SetLocalOrientation:
        {
            ds_msg::SetLocalOrientation setOrientationMsg;
            (*messages) >> setOrientationMsg;

            // Get component instance of entity to move
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setOrientationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set orientation of entity
                m_transformComponentManager.SetLocalOrientation(
                    transform, setOrientationMsg.localOrientation);

                // Get physics component instance of entity to move
                Instance phys = m_physicsComponentManager.GetInstanceForEntity(
                    setOrientationMsg.entity);

                // If has physics component
                if (phys.IsValid())
                {
                    // Get collision object from entity
                    btCollisionObject *collisionObject =
                        m_physicsComponentManager.GetCollisionObject(phys);

                    if (collisionObject != nullptr)
                    {
                        btTransform t;

                        t = collisionObject->getWorldTransform();

                        ds_math::Quaternion worldOrientation =
                            m_transformComponentManager.GetWorldOrientation(
                                transform);

                        t.setRotation(btQuaternion(
                            worldOrientation.x, worldOrientation.y,
                            worldOrientation.z, worldOrientation.w));

                        collisionObject->setWorldTransform(t);
                    }
                }
            }

            break;
        }
        case ds_msg::MessageType::SetLocalScale:
        {
            ds_msg::SetLocalScale setScaleMsg;
            (*messages) >> setScaleMsg;

            // Get component instance of entity to move
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setScaleMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set scale of entity
                m_transformComponentManager.SetLocalScale(
                    transform, setScaleMsg.localScale);
            }

            break;
        }
        case ds_msg::MessageType::DestroyEntity:
        {
            std::cout << "Received destroy entity message" << std::endl;

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
                // Remove collision object
                btCollisionObject *obj =
                    m_physicsComponentManager.GetCollisionObject(physics);
                btRigidBody *body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                    delete body->getMotionState();
                }

                m_dynamicsWorld->removeCollisionObject(obj);

                // Remove collision shape
                btCollisionShape *shape =
                    m_physicsComponentManager.GetCollisionShape(physics);
                m_collisionShapes.remove(shape);
                delete shape;

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

            // Terrain
            Instance terrain =
                m_terrainComponentManager.GetInstanceForEntity(e);

            if (terrain.IsValid())
            {
                m_terrainComponentManager.RemoveInstance(terrain);
            }

            break;
        }
        case ds_msg::MessageType::PauseEvent:
        {
            ds_msg::PauseEvent pauseMsg;
            (*messages) >> pauseMsg;

            m_isPaused = pauseMsg.shouldPause;

            break;
        }
        case ds_msg::MessageType::SetLinearVelocity:
        {
            ds_msg::SetLinearVelocity setVelocityMsg;
            (*messages) >> setVelocityMsg;

            // Get physics component of entity to set linear velocity of.
            Instance phys = m_physicsComponentManager.GetInstanceForEntity(
                setVelocityMsg.entity);

            if (phys.IsValid())
            {
                // Get the rigid body for this message
                btRigidBody *rigidBody =
                    (btRigidBody *)m_physicsComponentManager.GetCollisionObject(
                        phys);

                if (rigidBody != nullptr)
                {
                    ds_math::Vector3 linearVelocity = setVelocityMsg.velocity;
                    rigidBody->setLinearVelocity(btVector3(
                        linearVelocity.x, linearVelocity.y, linearVelocity.z));
                }
            }

            break;
        }
        default:
            messages->Extract(header.size);
            break;
        }
    }
}

void Physics::CreateRigidBody(Instance phys,
                              const ds_math::Vector3 &origin,
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
    if (colShapeType == "box")
    {
        colShape = new btBoxShape(
            btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z)));
    }
    else if (colShapeType == "sphere")
    {
        colShape = new btSphereShape(btScalar(scale.x));
    }

    if (colShape != nullptr)
    {
        m_collisionShapes.push_back(colShape);

        // Create rigid body
        btScalar bodyMass = btScalar(mass);

        // Rigid body is dynamic if and only if mass is non-zero,
        // otherwise
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

        rigidBody = new btRigidBody(rbInfo);

        // Add to physics component
        m_physicsComponentManager.SetCollisionObject(phys, rigidBody);

        // Add collision shape to physics component
        m_physicsComponentManager.SetCollisionShape(phys, colShape);

        // Add rigidbody to world
        m_dynamicsWorld->addRigidBody(rigidBody);
    }
    else
    {
        std::cerr << "Physics::CreateRigidBody: Unhandled collision "
                     "shape type: "
                  << colShapeType << std::endl;
    }
}

void Physics::CreateHeightMapRigidBody(Instance phys,
                                       const ds_math::Vector3 &origin,
                                       TerrainResource *terrainResource,
                                       float heightScale)
{
    btRigidBody *rigidBody = nullptr;

    // Create transform
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

    // Create collision shape
    btCollisionShape *colShape = nullptr;

    // Set height scale of terrain resource
    terrainResource->SetHeightScale(heightScale);

    // Copy terrain data - bullet doesn't copy data we pass to it,
    // we
    // need to
    // keep the data 'alive' for the lifetime of the physics system.
    m_heightmapData.push_back(terrainResource->GetHeightArray());

    if (terrainResource != nullptr)
    {
        colShape = new btHeightfieldTerrainShape(
            terrainResource->GetHeightmapWidth(),
            terrainResource->GetHeightmapHeight(), &m_heightmapData.back()[0],
            1, -0.5f * heightScale, 0.5f * heightScale, 1, PHY_FLOAT, false);
        // colShape->setLocalScaling(btVector3(2.0f, 2.0f, 2.0f));

        m_collisionShapes.push_back(colShape);

        btVector3 aabMin;
        btVector3 aabMax;
        colShape->getAabb(bodyTransform, aabMin, aabMax);

        if (colShape != nullptr)
        {
            // Create rigid body
            btScalar bodyMass = btScalar(0.0f);

            // Static
            btVector3 localInertia(0, 0, 0);

            btDefaultMotionState *bodyMotionState =
                new btDefaultMotionState(bodyTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(
                bodyMass, bodyMotionState, colShape, localInertia);

            rigidBody = new btRigidBody(rbInfo);

            // Add rigid body to physics component
            m_physicsComponentManager.SetCollisionObject(phys, rigidBody);

            // Add collision shape to physics component
            m_physicsComponentManager.SetCollisionShape(phys, colShape);

            // Add rigid body to world
            m_dynamicsWorld->addRigidBody(rigidBody);
        }
    }
}

void Physics::CreateGhostObject(Instance phys,
                                const ds_math::Vector3 &origin,
                                StringIntern::StringId shape,
                                const ds_math::Vector3 &scale)
{
    // return ghostObject;
    btRigidBody *rigidBody = nullptr;

    // Create transform
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

    // Create collision shape
    btCollisionShape *colShape = nullptr;
    std::string colShapeType = StringIntern::Instance().GetString(shape);
    if (colShapeType == "ghostBox")
    {
        colShape = new btBoxShape(
            btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z)));
        m_collisionShapes.push_back(colShape);
    }
    else if (colShapeType == "ghostSphere")
    {
        colShape = new btSphereShape(btScalar(scale.x));
        m_collisionShapes.push_back(colShape);
    }

    if (colShape != nullptr)
    {
        // Create rigid body
        btScalar bodyMass = 0.0f;

        // Rigid body is dynamic if and only if mass is non-zero,
        // otherwise
        // static
        bool isDynamic = (bodyMass != 0.0f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
        {
            colShape->calculateLocalInertia(bodyMass, localInertia);
        }

        btDefaultMotionState *bodyMotionState =
            new btDefaultMotionState(bodyTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(
            bodyMass, bodyMotionState, colShape, localInertia);

        rigidBody = new btRigidBody(rbInfo);

        // Make ghost
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() |
                                     btRigidBody::CF_NO_CONTACT_RESPONSE);

        // Add collision object to physics component
        m_physicsComponentManager.SetCollisionObject(phys, rigidBody);

        // Add collision shape to physics component
        m_physicsComponentManager.SetCollisionShape(phys, colShape);

        // Add ghost object to world
        m_dynamicsWorld->addRigidBody(rigidBody);
    }
    else
    {
        std::cerr << "Physics::CreateGhostObject: Unhandled collision "
                     "shape type: "
                  << colShapeType << std::endl;
    }
}

void Physics::CreatePlayerCapsule(Instance phys,
                                  const ds_math::Vector3 &origin,
                                  float radius,
                                  float height,
                                  float mass)
{
    btRigidBody *rigidBody = nullptr;

    // Create transform
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

    // Create collision shape
    btCollisionShape *colShape = nullptr;
    colShape = new btCapsuleShape(btScalar(radius), btScalar(height));

    if (colShape != nullptr)
    {
        m_collisionShapes.push_back(colShape);

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
        rbInfo.m_friction = 1.0f;

        rigidBody = new btRigidBody(rbInfo);

        rigidBody->setSleepingThresholds(0.0f, 0.0f);
        rigidBody->setAngularFactor(0.0f);

        // Add to physics component
        m_physicsComponentManager.SetCollisionObject(phys, rigidBody);

        // Add collision shape to physics component
        m_physicsComponentManager.SetCollisionShape(phys, colShape);

        // Add rigidbody to world
        m_dynamicsWorld->addRigidBody(rigidBody);
    }
    else
    {
        std::cerr << "Physics::CreatePlayerCapsule: Error creating "
                     "collision shape."
                  << std::endl;
    }
}
}
