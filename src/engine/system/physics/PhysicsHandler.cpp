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

		//TODO: remove rigid, delete motion state for each, delete rigid.
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


		// TODO: change shapes depending on what it is.
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

		// start at origin. we will set this later after init.
		btQuaternion startingRot(0, 0, 0, 1);
		btVector3 startingPos(0, 0, 0);

		btDefaultMotionState * groundMotionState =
			new btDefaultMotionState(btTransform(startingRot, startingPos));

		// TODO: Figure out base inert.
		// Mass set to 0 and inert set to 0 = immovable.
		btVector3 inert(0, 0, 0);
		int mass = 0;

		/*Anything fed into here is copied. Therefore, you can delete everything
		that makes up this construction object and save the construction object
		to create a prefab type system.*/

		btRigidBody::btRigidBodyConstructionInfo
			rigidConstructForEntity(mass, groundMotionState, colBounds, inert);


		
		btRigidBody * rigidBodyForEntity =
			new btRigidBody(rigidConstructForEntity);

		m_physicalWorld->addRigidBody(rigidBodyForEntity);

		// store this away so we can get info when required.
		// // entityID - assignedRigid.
		m_rigidCollection.insert(std::pair<int, btRigidBody*>(entityID, rigidBodyForEntity));

		// function to be called every tick.
		m_physicalWorld->setInternalTickCallback(SimulationTickCallback);

		delete groundMotionState;
		delete colBounds;
	}

	void PhysicsHandler::SetEntityPosition(int entityID, ds_math::Vector3 pos)
	{
		// may have to use motionstate to transition character
	}

	ds_math::Vector3 PhysicsHandler::GetEntityPosition(int entityID)
	{
		btTransform trans;
		m_rigidCollection[entityID]->getMotionState()->getWorldTransform(trans);
		float xPos = trans.getOrigin().getX();
		float yPos = trans.getOrigin().getY();
		float zPos = trans.getOrigin().getZ();

		ds_math::Vector3 position(xPos, yPos, zPos);

		return position;
	}

	void PhysicsHandler::ApplyRotationToEntity(int entityID, ds_math::Vector3 axis, float angle)
	{
		// may have to use motionstate to transition character
		// // sam's math class has rotation conversion from quat
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
		// TODO: move existing code from construct -> here.
	}

	void PhysicsHandler::SetEntityGravity(int entityID, ds_math::Vector3 grav)
	{
		// TODO: why is this an attribute? leave to later.
	}

	void PhysicsHandler::SetEntityAngDampening(int entityId, float damp)
	{
		// angular dampening - unsure what this is. later.
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
		// TODO: figure out steps based on fps
		m_physicalWorld->stepSimulation(deltaTime, 1, btScalar(1.0) / btScalar(60.));

		if (m_physicalWorld->getNumCollisionObjects() > 0)
		{
			// TODO: remove after testing.
			// or make this loop through all rigids and send out message
			// of new position.
			btTransform trans;
			m_rigidCollection[0]->getMotionState()->getWorldTransform(trans);
			std::cout << "height: " << trans.getOrigin().getY() << std::endl;
		}
		
	}

	void PhysicsHandler::SimulationTickCallback(btDynamicsWorld * world, btScalar timeStep)
	{
		//// To cycle through all col objects
		//btCollisionObjectArray objects = world->getCollisionObjectArray();
		//world->clearForces();
		//for (int i = 0; i < objects.size(); i++) {
		//	btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
		//	if (!rigidBody) {
		//		continue;
		//	}
		//	rigidBody->applyGravity();
		//	rigidBody->applyForce(btVector3(-10., 0., 0.), btVector3(0., 0., 0.));
		//}
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
