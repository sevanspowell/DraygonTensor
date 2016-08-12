#include <cassert>

#include "engine/system/custom_physics/CustomPhysicsComponentManager.h"

namespace ds
{
const ds_phys::Particle &
CustomPhysicsComponentManager::GetParticle(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
                    "Tried to get invalid instance.");

    return m_data.component[i.index].particle;
}

void CustomPhysicsComponentManager::SetParticle(
    Instance i, const ds_phys::Particle &particle)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
                    "Tried to set invalid instance.");

    m_data.component[i.index].particle = particle;
}
}
