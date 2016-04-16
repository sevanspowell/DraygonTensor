#include "PhysicsHandler.h"

ds::PhysicsHandler::PhysicsHandler()
{
}

ds::PhysicsHandler::~PhysicsHandler()
{
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

void ds::PhysicsHandler::SetWorldGravity(float gravAmount)
{
}

void ds::PhysicsHandler::SetEntityMess(int entityID, float mass)
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
