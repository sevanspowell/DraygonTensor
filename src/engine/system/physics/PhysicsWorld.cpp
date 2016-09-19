#include <algorithm>

#include "engine/system/physics/PhysicsWorld.h"

namespace ds_phys
{


static void setInertiaTensorCoeffs(ds_math::Matrix3& mat, ds_math::scalar ix, ds_math::scalar iy, ds_math::scalar iz,
		ds_math::scalar ixy=0, ds_math::scalar ixz=0, ds_math::scalar iyz=0) {
    mat[0][0] = ix;
    mat[0][1] =-ixy;
    mat[0][2] =-ixz;

    mat[1][0] = -ixy;
    mat[1][1] = iy;
    mat[1][2] = -iyz;

    mat[2][0] =-ixz;
    mat[2][1] = -iyz;
    mat[2][2] = iz;
}

static void setBlockInertiaTensor(ds_math::Matrix3& mat, const ds_math::Vector3 &halfSizes, ds_math::scalar mass) {
    ds_math::Vector3 squares = halfSizes*2;
    setInertiaTensorCoeffs(mat,
    	0.3f*mass*(squares.y + squares.z),
        0.3f*mass*(squares.x + squares.z),
        0.3f*mass*(squares.x + squares.y));
}

PhysicsWorld::PhysicsWorld(unsigned int maxContacts, unsigned int iterations) : m_contactResolver(10)
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
	//duration = 1/60.0f;

	//duration = duration;
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
    m_box2.calculateInternals();
    m_plane.calculateInternals();

    unsigned int got = generateContacts();
    std::cout << got << std::endl;
    std::cout << m_box.getTransform() << std::endl;
    //std::cout << m_plane.getTransform() << std::endl;
    std::cout << "---- Contacts generated -----" << std::endl;

    for (unsigned i = 0; i < got; ++i)
    {
        std::cout << m_contacts[i].contactPoint << " Pen: " << m_contacts[i].penetration << std::endl;
    }
    m_contactResolver.resolveContacts(m_contacts, got, duration);
    std::cout << "---- -----" << std::endl;

    // Resolve contacts
    // TODO: pass in generated contacts
}

void PhysicsWorld::addRigidBody(RigidBody *rigidBody)
{
	//@todo Remove when propper loading is done.
	rigidBody->setLinearDamping(0.9f);
	rigidBody->setAngularDamping(0.80f);
	rigidBody->clearAccumulators();
	rigidBody->setCanSleep(true);
	rigidBody->setAwake();
	rigidBody->setMass(1);
	ds_math::Matrix3 mat;
	setBlockInertiaTensor(mat, m_box2.halfSize, rigidBody->getMass());
	rigidBody->setInertiaTensor(mat);
	rigidBody->calculateDerivedData();

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
    m_collisionData.friction = (ds_math::scalar)0.1;
    m_collisionData.restitution = (ds_math::scalar)0.6;
    m_collisionData.tolerance = (ds_math::scalar)0.1;

    if (!m_collisionData.hasMoreContacts())
    {
        return m_collisionData.contactCount;
    }

    //CollisionDetector::boxAndHalfSpace(m_box, m_plane, &m_collisionData);
    CollisionDetector::boxAndHalfSpace(m_box, m_plane, &m_collisionData);
    CollisionDetector::boxAndBox(m_box, m_box2, &m_collisionData);
    CollisionDetector::boxAndHalfSpace(m_box2, m_plane, &m_collisionData);

    return m_collisionData.contactCount;
}
}
