#include <cassert>

#include "engine/system/physics/PhysicsComponentManager.h"

namespace ds
{
ds_phys::RigidBody *PhysicsComponentManager::GetRigidBody(Instance i)
{
    assert(i.index >= 0 && (unsigned)i.index < GetNumInstances() &&
           "Tried to get invalid instance.");

    return m_data.component[i.index].rigidBody;
}

void PhysicsComponentManager::SetRigidBody(Instance i, ds_phys::RigidBody *rigidBody)
{
    assert(i.index >= 0 && (unsigned)i.index < GetNumInstances() &&
           "Tried to set invalid instance");

    m_data.component[i.index].rigidBody = rigidBody;
}
}
