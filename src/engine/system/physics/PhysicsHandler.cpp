#pragma once

#include "engine/system/physics/PhysicsHandler.h"
#include "engine/resource/MeshResource.h"


namespace ds
{
	PhysicsHandler::PhysicsHandler()
	{

		SetupPhysicsWorld();
		m_factory.RegisterCreator<MeshResource>(
			MeshResource::CreateFromFile);
	}

	PhysicsHandler::~PhysicsHandler()
	{
		delete m_physicalWorld;
		delete m_broadphaseAlgo;
		delete m_worldLogic;
		delete m_collisionConfig;
		delete m_dispatcher;

	}

	void PhysicsHandler::ConstructEntity(int entityID, std::string modelPath, bool staticObject)
	{
		std::unique_ptr<MeshResource> meshUsedForEntity =
				m_factory.CreateResource<MeshResource>(modelPath);

		btConvexHullShape * colBounds = new btConvexHullShape();

		int numOfPoints = meshUsedForEntity->GetIndicesCount();

		for (int iTriPoint = 0; iTriPoint < numOfPoints; iTriPoint++)
		{
			ds_math::Vector3 pointDetails = (meshUsedForEntity->GetIndices())[iTriPoint];
			float meshX = pointDetails.x;
			float meshY = pointDetails.y;
			float meshZ = pointDetails.z;
			btVector3 pointLoc(meshX, meshY, meshZ);

			colBounds->addPoint(pointLoc);
		}

		btQuaternion startingRot(0, 0, 0, 1);
		btVector3 startingPos(0, 0, 0);

		btDefaultMotionState * groundMotionState =
			new btDefaultMotionState(btTransform(startingRot, startingPos));

		btVector3 inert(0, 0, 0);
		int mass = 0;

		btRigidBody::btRigidBodyConstructionInfo
			rigidConstructForEntity(mass, groundMotionState, colBounds, inert);


		
		btRigidBody * rigidBodyForEntity =
			new btRigidBody(rigidConstructForEntity);

		m_physicalWorld->addRigidBody(rigidBodyForEntity);

		delete groundMotionState;
		delete colBounds;
	}

	void PhysicsHandler::SetEntityPosition(int entityID, ds_math::Vector3 pos)
	{
	}

	ds_math::Vector3 PhysicsHandler::GetEntityPosition(int entityID)
	{
		return ds_math::Vector3();
	}

	void PhysicsHandler::ApplyRotationToEntity(int entityID, ds_math::Vector3 axis, float angle)
	{
	}

	void PhysicsHandler::SetWorldGravity(float yAmount)
	{
		m_physicalWorld->setGravity(btVector3(0, yAmount, 0));
	}

	void PhysicsHandler::SetWorldGravity(ds_math::Vector3 gravVec)
	{
		float xGrav = gravVec.x;
		float yGrav = gravVec.y;
		float zGrav = gravVec.z;

		m_physicalWorld->setGravity(btVector3(xGrav, yGrav, zGrav));
	}

	void PhysicsHandler::SetEntityMesh(int entityID, float mass)
	{
	}

	void PhysicsHandler::SetEntityGravity(int entityID, ds_math::Vector3 grav)
	{
	}

	void PhysicsHandler::SetEntityAngDampening(int entityId, float damp)
	{
	}

	void PhysicsHandler::SetEntityFriction(int entityID, float fric)
	{
	}

	void PhysicsHandler::RemoveEntity(int entityID)
	{
	}

	bool PhysicsHandler::DoesEntityExist(int entityID)
	{
		return false;
	}

	void PhysicsHandler::SetPhysicsWorldIntertia(float inert)
	{
	}

	void PhysicsHandler::UpdateWorldSimulation(float deltaTime)
	{
		m_physicalWorld->stepSimulation(deltaTime, 1, btScalar(1.0) / btScalar(60.));
	}

	void PhysicsHandler::SetupPhysicsWorld()
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
}
