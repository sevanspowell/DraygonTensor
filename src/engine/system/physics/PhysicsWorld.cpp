#include <algorithm>

#include "engine/system/physics/PhysicsWorld.h"

namespace ds_phys
{


PhysicsWorld::PhysicsWorld(unsigned int maxContacts, unsigned int iterations)
    : m_currentCPID(0), m_contactResolver(1024)
{
    m_rigidBodies.reserve(100);
    // m_collisionConfiguration = new btDefaultCollisionConfiguration();
    // m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    // m_broadPhase = new btDbvtBroadphase();
    // m_collisionWorld = new btCollisionWorld(m_dispatcher, m_broadphase,
    //                                         m_collisionConfiguration);
    m_collisionData.contactArray = m_contacts;
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

    // rigidBody->setRotation(ds_math::Vector3(0, 0, 0));
    // rigidBody->setLinearDamping(0.95f);
    // rigidBody->setAngularDamping(0.80f);
    rigidBody->clearAccumulators();
    // rigidBody->setAcceleration(0.0f, -10.0f, 0.0f);
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

void PhysicsWorld::addForceGenerator(
    RigidBody *rigidBody, std::shared_ptr<IForceGenerator> forceGenerator)
{
    m_forceRegistry.add(rigidBody, forceGenerator);
}

void PhysicsWorld::removeForceGenerator(
    RigidBody *rigidBody, std::shared_ptr<IForceGenerator> forceGenerator)
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

static unsigned generateCollisions(CollisionPrimitive* b0, CollisionPrimitive* b1, CollisionData& data) {
	if (!data.hasMoreContacts()) return 0;
	if (!b0) return 0;
	if (!b1) return 0;
	if (b0->body) {
		if (b1->body) {
			if ((!b0->body->hasFiniteMass()) && (!b1->body->hasFiniteMass())) return 0;
		} else {
			if (!b0->body->hasFiniteMass()) return 0;
		}
	} else {
		if (b1->body) {
			if (!b1->body->hasFiniteMass()) return 0;
		}
	}
	if ((b0->body == b1->body) && (b0->body != nullptr)) return 0;


    if (CollisionBox *b0Ptr = dynamic_cast<CollisionBox *>(b0)) {
    	if (CollisionBox *b1Ptr = dynamic_cast<CollisionBox *>(b1)) {
            return CollisionDetector::boxAndBox(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionSphere *b1Ptr = dynamic_cast<CollisionSphere *>(b1))
        {
            return CollisionDetector::boxAndSphere(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionPlane *b1Ptr = dynamic_cast<CollisionPlane *>(b1))
        {
            return CollisionDetector::boxAndHalfSpace(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionCapsule *b1Ptr = dynamic_cast<CollisionCapsule *>(b1))
        {
            return CollisionDetector::capsuleAndBox(*b1Ptr, *b0Ptr, &data);
        }
    }
    else if (CollisionSphere *b0Ptr = dynamic_cast<CollisionSphere *>(b0))
    {
        if (CollisionBox *b1Ptr = dynamic_cast<CollisionBox *>(b1))
        {
            return CollisionDetector::boxAndSphere(*b1Ptr, *b0Ptr, &data);
        }
        else if (CollisionSphere *b1Ptr = dynamic_cast<CollisionSphere *>(b1))
        {
            return CollisionDetector::sphereAndSphere(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionPlane *b1Ptr = dynamic_cast<CollisionPlane *>(b1))
        {
            return CollisionDetector::sphereAndHalfSpace(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionCapsule *b1Ptr = dynamic_cast<CollisionCapsule *>(b1))
        {
            return CollisionDetector::capsuleAndSphere(*b1Ptr, *b0Ptr, &data);
        }
    }
    else if (CollisionPlane *b0Ptr = dynamic_cast<CollisionPlane *>(b0))
    {
        if (CollisionBox *b1Ptr = dynamic_cast<CollisionBox *>(b1))
        {
            return CollisionDetector::boxAndHalfSpace(*b1Ptr, *b0Ptr, &data);
        }
        else if (CollisionSphere *b1Ptr = dynamic_cast<CollisionSphere *>(b1))
        {
            return CollisionDetector::sphereAndHalfSpace(*b1Ptr, *b0Ptr, &data);
        }
        else if (/*CollisionPlane *b1Ptr = */ dynamic_cast<CollisionPlane *>(
            b1))
        {
            // Skip
        }
        else if (CollisionCapsule *b1Ptr = dynamic_cast<CollisionCapsule *>(b1))
        {
            return CollisionDetector::capsuleAndHalfSpace(*b1Ptr, *b0Ptr,
                                                          &data);
        }
    }
    else if (CollisionCapsule *b0Ptr = dynamic_cast<CollisionCapsule *>(b0))
    {
        if (CollisionBox *b1Ptr = dynamic_cast<CollisionBox *>(b1))
        {
            return CollisionDetector::capsuleAndBox(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionSphere *b1Ptr = dynamic_cast<CollisionSphere *>(b1))
        {
            return CollisionDetector::capsuleAndSphere(*b0Ptr, *b1Ptr, &data);
        }
        else if (CollisionPlane *b1Ptr = dynamic_cast<CollisionPlane *>(b1))
        {
            return CollisionDetector::capsuleAndHalfSpace(*b0Ptr, *b1Ptr,
                                                          &data);
        }
        else if (CollisionCapsule *b1Ptr = dynamic_cast<CollisionCapsule *>(b1))
        {
            return CollisionDetector::capsuleAndCapsule(*b0Ptr, *b1Ptr, &data);
        }
    }

    return 0;
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
            auto iter2 = iter;
            for (++iter2; iter2 != m_collisionPrimitives.end(); iter2++)
            {
                if (iter2->second)
                {
                    auto *cpPtr2 = iter2->second.get();
                    generateCollisions(cpPtr1, cpPtr2, m_collisionData);
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
