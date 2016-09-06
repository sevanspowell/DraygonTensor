#include <algorithm>

#include "engine/system/physics/PhysicsWorld.h"

namespace ds_phys
{
PhysicsWorld::PhysicsWorld(unsigned int maxContacts, unsigned int iterations)
{
    // m_collisionConfiguration = new btDefaultCollisionConfiguration();
    // m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    // m_broadPhase = new btDbvtBroadphase();
    // m_collisionWorld = new btCollisionWorld(m_dispatcher, m_broadphase,
    //                                         m_collisionConfiguration);
    m_collisionData.contactArray = m_contacts;

    m_box.halfSize = ds_math::Vector3(0.5f, 0.5f, 0.5f);
    m_plane.body = nullptr;
    m_plane.direction = ds_math::Vector3::Normalize(ds_math::Vector3(1, 1, 0));
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
                  [](RigidBody *rigidBody)
                  {
                      if (rigidBody != nullptr)
                      {
                          rigidBody->clearAccumulators();
                          rigidBody->calculateDerivedData();
                      }
                  });
}

void PhysicsWorld::stepSimulation(ds_math::scalar duration)
{
    // Update force generators
    m_forceRegistry.updateForces(duration);

    // Integrate rigid bodies
    std::for_each(m_rigidBodies.begin(), m_rigidBodies.end(),
                  [&](RigidBody *rigidBody)
                  {
                      if (rigidBody != nullptr)
                      {
                          rigidBody->integrate(duration);
                      }
                  });

    // Generate contacts
    // m_collisionWorld->performDiscreteCollisionDetection();
    m_box.calculateInternals();
    m_plane.calculateInternals();
    unsigned int usedContacts = generateContacts();

    if (usedContacts > 0)
    {
        m_contactResolver.setIterations(usedContacts * 2);
        m_contactResolver.resolveContacts(m_contacts, usedContacts, duration);
    }

    // Resolve contacts
    // TODO: pass in generated contacts
    // m_contactResolver.resolveContacts()
}

void PhysicsWorld::addRigidBody(RigidBody *rigidBody)
{
    m_rigidBodies.push_back(rigidBody);
}

void PhysicsWorld::removeRigidBody(RigidBody *rigidBody)
{
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

    CollisionDetector::boxAndHalfSpace(m_box, m_plane, &m_collisionData);

    return m_collisionData.contactCount;
}
}
