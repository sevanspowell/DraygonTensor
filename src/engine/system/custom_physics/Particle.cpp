#include <cassert>

#include "Particle.h"
#include "math/Precision.h"

namespace ds_phys
{
void Particle::setMass(const ds_math::scalar mass)
{
    assert(mass != 0);
    m_inverseMass = ((ds_math::scalar)1.0) / mass;
}

ds_math::scalar Particle::getMass() const
{
    if (m_inverseMass == 0)
    {
        return ds_math::SCALAR_MAX;
    }
    else
    {
        return ((ds_math::scalar)1.0) / m_inverseMass;
    }
}

void Particle::setInverseMass(const ds_math::scalar invMass)
{
    m_inverseMass = invMass;
}

ds_math::scalar Particle::getInverseMass() const
{
    return m_inverseMass;
}

bool Particle::hasFiniteMass() const
{
    return (m_inverseMass > 0.0f);
}

void Particle::setDamping(const ds_math::scalar damping)
{
    m_damping = damping;
}

ds_math::scalar Particle::getDamping() const
{
    return m_damping;
}

void Particle::setPosition(const ds_math::Vector3 &position)
{
    m_position = position;
}

void Particle::setPosition(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

const ds_math::Vector3 &Particle::getPosition() const
{
    return m_position;
}

void Particle::setVelocity(const ds_math::Vector3 &velocity)
{
    m_velocity = velocity;
}

void Particle::setVelocity(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

const ds_math::Vector3 &Particle::getVelocity() const
{
    return m_velocity;
}

void Particle::setAcceleration(const ds_math::Vector3 &acceleration)
{
    std::cout << "Accel: " << acceleration << std::endl;

    m_acceleration = acceleration;
}

void Particle::setAcceleration(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    m_acceleration.x = x;
    m_acceleration.y = y;
    m_acceleration.z = z;
}

const ds_math::Vector3 &Particle::getAcceleration() const
{
    return m_acceleration;
}

void Particle::integrate(ds_math::scalar duration)
{
    assert(duration > 0.0);

    // Update linear position.
    m_position += m_velocity * duration;

    // Work out the acceleration from the force.
    ds_math::Vector3 resultingAcceleration =
        m_acceleration + (m_forceAccum * m_inverseMass);

    // Update linear velocity from the acceleration.
    m_velocity += resultingAcceleration * duration;

    // Impose drag.
    m_velocity *= scalarPow(m_damping, duration);

    // Clear accumulated forces
    clearAccumulator();
}

void Particle::clearAccumulator()
{
    m_forceAccum.x = 0;
    m_forceAccum.y = 0;
    m_forceAccum.z = 0;
}

void Particle::addForce(const ds_math::Vector3 &force)
{
    m_forceAccum += force;
}
}
