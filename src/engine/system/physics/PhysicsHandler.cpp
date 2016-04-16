#include "PhysicsHandler.h"

ds::PhysicsHandler::PhysicsHandler()
{
	m_broadphaseAlgo =
		new btDbvtBroadphase();

	m_collisionConfig =
		new btDefaultCollisionConfiguration();

	m_dispatcher =
		new btCollisionDispatcher(m_collisionConfig);


	m_worldLogic =
		new btSequentialImpulseConstraintSolver();

	m_physicalWorld = 
		new btDiscreteDynamicsWorld(m_dispatcher, m_broadphaseAlgo,
			m_worldLogic, m_collisionConfig);
}

ds::PhysicsHandler::~PhysicsHandler()
{
	delete m_physicalWorld;
	delete m_broadphaseAlgo;
	delete m_worldLogic;
	delete m_collisionConfig;
	delete m_dispatcher;

}

void ds::PhysicsHandler::ConstructEntity(int entityID, std::string modelPath, bool staticObject)
{
}

void ds::PhysicsHandler::SetEntityPosition(int entityID, ds_math::Vector3 pos)
{
}

ds_math::Vector3 ds::PhysicsHandler::GetEntityPosition(int entityID)
{
	return ds_math::Vector3();
}

void ds::PhysicsHandler::ApplyRotationToEntity(int entityID, ds_math::Vector3 axis, float angle)
{
}

void ds::PhysicsHandler::SetWorldGravity(float yAmount)
{
	m_physicalWorld->setGravity(btVector3(0, yAmount, 0));
}

void ds::PhysicsHandler::SetWorldGravity(ds_math::Vector3 gravVec)
{
	float xGrav = gravVec.x;
	float yGrav = gravVec.y;
	float zGrav = gravVec.z;

	m_physicalWorld->setGravity(btVector3(xGrav, yGrav, zGrav));
}

void ds::PhysicsHandler::SetEntityMesh(int entityID, float mass)
{
}

void ds::PhysicsHandler::SetEntityGravity(int entityID, ds_math::Vector3 grav)
{
}

void ds::PhysicsHandler::SetEntityAngDampening(int entityId, float damp)
{
}

void ds::PhysicsHandler::SetEntityFriction(int entityID, float fric)
{
}

void ds::PhysicsHandler::RemoveEntity(int entityID)
{
}

bool ds::PhysicsHandler::DoesEntityExist(int entityID)
{
	return false;
}

void ds::PhysicsHandler::SetPhysicsWorldIntertia(float inert)
{
}
