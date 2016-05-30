#include <cassert>

#include "engine/system/physics/PhysicsComponentManager.h"

namespace ds
{
btCollisionObject *PhysicsComponentManager::GetCollisionObject(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::GetCollisionObject: tried to get invalid "
           "instance");

    return m_data.component[i.index].collisionObject;
}

void PhysicsComponentManager::SetCollisionObject(
    Instance i, btCollisionObject *collisionObject)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::SetCollisionObject: tried to set invalid "
           "instance");

    m_data.component[i.index].collisionObject = collisionObject;
}

btCollisionShape *PhysicsComponentManager::GetCollisionShape(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::GetCollisionShape: tried to get invalid "
           "instance");

    return m_data.component[i.index].collisionShape;
}

void PhysicsComponentManager::SetCollisionShape(
    Instance i, btCollisionShape *collisionShape)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::SetCollisionShape: tried to set invalid "
           "instance");

    m_data.component[i.index].collisionShape = collisionShape;
}

StringIntern::StringId PhysicsComponentManager::GetShape(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::GetShape: tried to get invalid instance");

    return m_data.component[i.index].shape;
}

void PhysicsComponentManager::SetShape(Instance i, StringIntern::StringId shape)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::SetShape: tried to set invalid instance");

    m_data.component[i.index].shape = shape;
}

const ds_math::Vector3 &PhysicsComponentManager::GetScale(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::GetScale: tried to get invalid instance");

    return m_data.component[i.index].scale;
}

void PhysicsComponentManager::SetScale(Instance i,
                                       const ds_math::Vector3 &scale)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::SetScale: tried to set invalid instance");

    m_data.component[i.index].scale = scale;
}

float PhysicsComponentManager::GetMass(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::GetMass: tried to get invalid instance");

    return m_data.component[i.index].mass;
}

void PhysicsComponentManager::SetMass(Instance i, float mass)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "PhysicsComponentManager::SetMass: tried to set invalid instance");

    m_data.component[i.index].mass = mass;
}

Instance PhysicsComponentManager::GetInstanceForCollisionObject(
    const btCollisionObject *collisionObject)
{
    Instance phys;

    // Iterate thru physics component instances
    for (unsigned int i = 0; i < GetNumInstances(); ++i)
    {
        if (m_data.component[i].collisionObject == collisionObject)
        {
            phys = Instance::MakeInstance(i);
            break;
        }
    }

    return phys;
}
}
