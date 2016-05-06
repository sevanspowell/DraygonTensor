#include <cassert>

#include "engine/system/physics/PhysicsComponentManager.h"

namespace ds
{
btRigidBody *PhysicsComponentManager::GetRigidBody(Instance i) const
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::GetRigidBody: tried to get invalid instance");

    return m_data.component[i.index].rigidBody;
}

void PhysicsComponentManager::SetRigidBody(Instance i,
                                           btRigidBody *rigidBody)
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::SetRigidBody: tried to set invalid instance");

    m_data.component[i.index].rigidBody = rigidBody;
}


StringIntern::StringId PhysicsComponentManager::GetShape(Instance i) const
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::GetShape: tried to get invalid instance");

    return m_data.component[i.index].shape;
}

void PhysicsComponentManager::SetShape(Instance i, StringIntern::StringId shape)
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::SetShape: tried to set invalid instance");

    m_data.component[i.index].shape = shape;
}

const ds_math::Vector3 &PhysicsComponentManager::GetScale(Instance i) const
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::GetScale: tried to get invalid instance");

    return m_data.component[i.index].scale;
}

void PhysicsComponentManager::SetScale(Instance i,
                                       const ds_math::Vector3 &scale)
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::SetScale: tried to set invalid instance");

    m_data.component[i.index].scale = scale;
}

float PhysicsComponentManager::GetMass(Instance i) const
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::GetMass: tried to get invalid instance");

    return m_data.component[i.index].mass;
}

void PhysicsComponentManager::SetMass(Instance i, float mass)
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "PhysicsComponentManager::SetMass: tried to set invalid instance");

    m_data.component[i.index].mass = mass;
}
}
