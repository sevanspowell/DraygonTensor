#include "engine/system/physics/RigidBody.h"

namespace ds_phys
{
void RigidBody::integrate(ds_math::scalar duration)
{
}

void RigidBody::clearAccumulators()
{
}

void RigidBody::calculateDerivedData()
{
}

ds_math::Vector3 RigidBody::getPosition() const
{
    return m_position;
}

void RigidBody::setPosition(const ds_math::Vector3 &position)
{
    m_position = position;
}
}
