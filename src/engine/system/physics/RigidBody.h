#pragma once

#include "math/Vector3.h"

namespace ds_phys
{
class RigidBody
{
public:
    void integrate(ds_math::scalar duration);

    void clearAccumulators();

    void calculateDerivedData();

    ds_math::Vector3 getPosition() const;

    void setPosition(const ds_math::Vector3 &position);

protected:
    ds_math::Vector3 m_position;

private:
};
}
