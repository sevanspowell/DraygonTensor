#include <algorithm>

#include "engine/system/physics/PhysicsWorld.h"

namespace ds_phys
{



PhysicsWorld::PhysicsWorld(unsigned int maxContacts, unsigned int iterations)
    : m_currentCPID(0), m_contactResolver(10)
{
    m_rigidBodies.reserve(100);
    // m_collisionConfiguration = new btDefaultCollisionConfiguration();
    // m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    // m_broadPhase = new btDbvtBroadphase();
    // m_collisionWorld = new btCollisionWorld(m_dispatcher, m_broadphase,
    //                                         m_collisionConfiguration);
    m_collisionData.contactArray = m_contacts;

    m_box.halfSize = ds_math::Vector3(0.5f, 0.5f, 0.5f);
    m_box2.halfSize = ds_math::Vector3(0.5f, 0.5f, 0.5f);

    m_sphere.radius = 0.5;

    m_plane.body = nullptr;
    m_plane.direction = ds_math::Vector3::Normalize(ds_math::Vector3(0, 1, 0));
    m_plane.offset = 0.0f;
}

PhysicsWorld::~PhysicsWorld()
{
    // // Delete collision world;
    // delete m_collisionWorld;

    // // Delete broadPhase
    // delete m_broadPhase;

    // // Delete dispatcher
    // delete m_dispatcher;

    // // Delete collision configuration
    // delete m_collisionConfiguration;
}

void PhysicsWorld::startFrame()
{
    // Remove all forces from accumulators in each rigidbody
    std::for_each(m_rigidBodies.begin(), m_rigidBodies.end(),
                  [](RigidBody *rigidBody) {
                      if (rigidBody != nullptr)
                      {
                          rigidBody->clearAccumulators();
                          rigidBody->calculateDerivedData();
                      }
                  });
}

void PhysicsWorld::stepSimulation(ds_math::scalar duration)
{
    // duration = duration;
    // Update force generators
    m_forceRegistry.updateForces(duration);

    // Integrate rigid bodies
    std::for_each(m_rigidBodies.begin(), m_rigidBodies.end(),
                  [&](RigidBody *rigidBody) {
                      if (rigidBody != nullptr)
                      {
                          rigidBody->integrate(duration);
                      }
                  });

    unsigned int got = generateContacts();

    // Resolve contacts
    m_contactResolver.resolveContacts(m_contacts, got, duration);
}

void PhysicsWorld::addRigidBody(RigidBody *rigidBody)
{
    //@todo Remove when propper loading is done.
    rigidBody->setRotation(ds_math::Vector3(0, 0, 0));

    rigidBody->setLinearDamping(0.95f);
    rigidBody->setAngularDamping(0.80f);
    rigidBody->clearAccumulators();
    rigidBody->setAcceleration(0.0f, -10.0f, 0.0f);
    rigidBody->setCanSleep(true);

    rigidBody->setAwake();

    rigidBody->calculateDerivedData();

    m_rigidBodies.push_back(rigidBody);
}

void PhysicsWorld::removeRigidBody(RigidBody *rigidBody)
{
    while (rigidBody->getCollisionPrimitiveCount() > 0)
    {
        removeCollisionPrimitive(rigidBody->getCollisionPrimitive(0));
        rigidBody->removeCollisionPrimitive(0);
    }

    std::vector<RigidBody *>::iterator it =
        std::find(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody);

    if (it != m_rigidBodies.end())
    {
        // TODO: Leaves gap in memory, look at not using RigidBody *, for cache
        // coherency
        m_rigidBodies.erase(it);
    }
}

void PhysicsWorld::addForceGenerator(RigidBody *rigidBody,
                                     IForceGenerator *forceGenerator)
{
    m_forceRegistry.add(rigidBody, forceGenerator);
}

void PhysicsWorld::removeForceGenerator(RigidBody *rigidBody,
                                        IForceGenerator *forceGenerator)
{
    m_forceRegistry.remove(rigidBody, forceGenerator);
}

CollisionPrimitiveID PhysicsWorld::addCollisionPrimitive(
    std::unique_ptr<CollisionPrimitive> &&primitive)
{
    m_collisionPrimitives[++m_currentCPID] = std::move(primitive);
    return m_currentCPID;
}

CollisionPrimitive *PhysicsWorld::getCollisionPrimitive(CollisionPrimitiveID id)
{
    auto iter = m_collisionPrimitives.find(id);
    if (iter != m_collisionPrimitives.end())
    {
        return iter->second.get();
    }
    return nullptr;
}

CollisionPrimitiveID
PhysicsWorld::getCollisionPrimitiveID(CollisionPrimitive *primitive)
{
    for (auto iter = m_collisionPrimitives.begin();
         iter != m_collisionPrimitives.end(); iter++)
    {
        if (primitive == iter->second.get())
        {
            return iter->first;
        }
    }
    return 0;
}

std::unique_ptr<CollisionPrimitive>
PhysicsWorld::removeCollisionPrimitive(CollisionPrimitiveID id)
{
    auto iter = m_collisionPrimitives.find(id);
    if (iter != m_collisionPrimitives.end())
    {
        auto tmp = std::move(iter->second);
        m_collisionPrimitives.erase(iter);
        return tmp;
    }
    return std::unique_ptr<CollisionPrimitive>();
}

std::unique_ptr<CollisionPrimitive>
PhysicsWorld::removeCollisionPrimitive(CollisionPrimitive *primitive)
{
    for (auto iter = m_collisionPrimitives.begin();
         iter != m_collisionPrimitives.end(); iter++)
    {
        if (primitive == iter->second.get())
        {
            auto tmp = std::move(iter->second);
            m_collisionPrimitives.erase(iter);
            return tmp;
        }
    }
    return std::unique_ptr<CollisionPrimitive>();
}

unsigned int PhysicsWorld::generateContacts()
{
    m_collisionData.reset(PhysicsWorld::MAX_CONTACTS);
    m_collisionData.friction = (ds_math::scalar)0.9;
    m_collisionData.restitution = (ds_math::scalar)0.6;
    m_collisionData.tolerance = (ds_math::scalar)0.1;

    if (!m_collisionData.hasMoreContacts())
    {
        return m_collisionData.contactCount;
    }

    m_plane.calculateInternals();

    // Calculate internals for all rigid bodies
    for (auto iter = m_collisionPrimitives.begin();
         iter != m_collisionPrimitives.end(); iter++)
    {
        if (iter->second)
        {
            iter->second->calculateInternals();
        }
    }

    // Calculate internals for all rigid bodies
    for (auto iter = m_collisionPrimitives.begin();
         iter != m_collisionPrimitives.end(); iter++)
    {
        if (iter->second)
        {
            auto *cpPtr1 = iter->second.get();
            if (CollisionBox *cbBox1 = dynamic_cast<CollisionBox *>(cpPtr1))
            {
                CollisionDetector::boxAndHalfSpace(*cbBox1, m_plane,
                                                   &m_collisionData);
                auto iter2 = iter;
                for (++iter2; iter2 != m_collisionPrimitives.end(); iter2++)
                {
                    if (iter->second)
                    {
                        auto *cpPtr2 = iter2->second.get();
                        if (CollisionBox *cbBox2 =
                                dynamic_cast<CollisionBox *>(cpPtr2))
                        {
                            CollisionDetector::boxAndBox(*cbBox1, *cbBox2,
                                                         &m_collisionData);
                        }
                        else if (CollisionSphere *cbSphere2 =
                                     dynamic_cast<CollisionSphere *>(cpPtr2))
                        {
                            CollisionDetector::boxAndSphere(*cbBox1, *cbSphere2,
                                                            &m_collisionData);
                        }
                    }
                }
            }
            else if (CollisionSphere *cbSphere1 =
                         dynamic_cast<CollisionSphere *>(cpPtr1))
            {
                CollisionDetector::sphereAndHalfSpace(*cbSphere1, m_plane,
                                                      &m_collisionData);
                auto iter2 = iter;
                for (++iter2; iter2 != m_collisionPrimitives.end(); iter2++)
                {
                    if (iter->second)
                    {
                        auto *cpPtr2 = iter2->second.get();
                        if (CollisionBox *cbBox2 =
                                dynamic_cast<CollisionBox *>(cpPtr2))
                        {
                            CollisionDetector::boxAndSphere(*cbBox2, *cbSphere1,
                                                            &m_collisionData);
                        }
                        else if (CollisionSphere *cbSphere2 =
                                     dynamic_cast<CollisionSphere *>(cpPtr2))
                        {
                            CollisionDetector::sphereAndSphere(
                                *cbSphere1, *cbSphere2, &m_collisionData);
                        }
                    }
                }
            }
        }
    }

    // CollisionDetector::boxAndHalfSpace(m_box, m_plane, &m_collisionData);
    // CollisionDetector::boxAndHalfSpace(m_box2, m_plane, &m_collisionData);
    // CollisionDetector::boxAndBox(m_box, m_box2, &m_collisionData);

    // CollisionDetector::sphereAndHalfSpace(m_sphere, m_plane,
    // &m_collisionData);

    return m_collisionData.contactCount;
}
}
