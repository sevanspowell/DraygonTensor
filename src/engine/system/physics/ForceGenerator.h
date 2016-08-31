#pragma once

#include <vector>

#include "math/Precision.h"

#include "engine/system/physics/RigidBody.h"

namespace ds_phys
{
class IForceGenerator
{
public:
    virtual void updateForce(RigidBody *body, ds_math::scalar duration) = 0;
};

class Gravity : public IForceGenerator
{
public:
private:
};

class GoDown : public IForceGenerator
{
public:
    GoDown(float speed);

    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

private:
    float m_speed;
};

class ForceRegistry
{
public:
    void add(RigidBody *body, IForceGenerator *fg);

    void remove(RigidBody *body, IForceGenerator *fg);

    void clear();

    void updateForces(ds_math::scalar duration);

protected:
    struct ForceRegistration
    {
        RigidBody *body;
        IForceGenerator *fg;
    };

    typedef std::vector<ForceRegistration> Registry;
    Registry m_registrations;

private:
};
}
