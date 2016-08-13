/*
 * The MIT License
 *
 * Copyright (c) 2003-2009 Ian Millington
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Modified by Samuel Evans-Powell
 */
#pragma once

#include "math/Vector3.h"

namespace ds_phys
{
/**
 * A particle is the simplest object that can be simulated in the physics
 * system.
 */
class Particle
{
public:
    /**
     * Sets the mass of the particle.
     *
     * @warning This invalidates internal data for the particle. Either an
     * integration methods, or the calculateInternals() method should be called
     * before trying to get any settings from the particle.
     *
     * @param   mass   const ds_math::scalar, the new mass of the body. May not be 0.
     * Small masses can produce unstable rigid bodies under simulation.
     */
    void setMass(const ds_math::scalar mass);

    /**
     * Gets the mass of the particle.
     *
     * @return   ds_math::scalar, current mass of the particle.
     */
    ds_math::scalar getMass() const;

    /**
     * Sets the inverse mass of the particle.
     *
     * @warning This invalidates internal data for the particle. Either an
     * integration methods, or the calculateInternals() method should be called
     * before trying to get any settings from the particle.
     *
     * @param   invMass   const ds_math::scalar, the new inverse mass of the body. This
     * may be zero, for a body with infinite mass (i.e. an immovable object).
     */
    void setInverseMass(const ds_math::scalar invMass);

    /**
     * Gets the inverse mass of the particle.
     *
     * @return   ds_math::scalar, current inverse mass of the particle.
     */
    ds_math::scalar getInverseMass() const;

    /**
     * Returns true if the mass of the particle is not-infinite, false
     * otherwise.
     *
     * @return   bool, true if the mass of the particle is not-infinite, false
     * otherwise.
     */
    bool hasFiniteMass() const;

    /**
     * Sets the damping applied to the particle's motion.
     *
     * @param   ds_math::scalar, damping to apply to the particle's motion.
     */
    void setDamping(const ds_math::scalar damping);

    /**
     * Gets the current damping value.
     *
     * @return   ds_math::scalar, current damping value.
     */
    ds_math::scalar getDamping() const;

    /**
     * Sets the position of the particle.
     *
     * @param   position   const ds_math::Vector3 &, new position of the
     * particle.
     */
    void setPosition(const ds_math::Vector3 &position);

    /**
     * Sets the position of the particle by component.
     *
     * @param   x   const ds_math::scalar, new x co-ordinate of the particle.
     * @param   y   const ds_math::scalar, new y co-ordinate of the particle.
     * @param   z   const ds_math::scalar, new z co-ordinate of the particle.
     */
    void setPosition(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

    /**
     * Gets the position of the particle.
     *
     * @return   const ds_math::Vector3 &, position of the particle.
     */
    const ds_math::Vector3 &getPosition() const;

    /**
     * Sets the velocity of the particle.
     *
     * @param   const ds_math::Vector3 &, new velocity of the particle.
     */
    void setVelocity(const ds_math::Vector3 &velocity);

    /**
     * Sets the velocity of the particle by component.
     *
     * @param   x   const ds_math::scalar, new x velocity of the particle.
     * @param   y   const ds_math::scalar, new y velocity of the particle.
     * @param   z   const ds_math::scalar, new z velocity of the particle.
     */
    void setVelocity(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

    /**
     * Gets the current velocity of the particle.
     *
     * @return const ds_math::Vector3 &, current velocity of the particle.
     */
    const ds_math::Vector3 &getVelocity() const;

    /**
     * Sets constant acceleration of the particle.
     *
     * @param   acceleration   const ds_math::Vector3 &, new acceleration of the
     * particle.
     */
    void setAcceleration(const ds_math::Vector3 &acceleration);

    /**
     * Sets the constant acceleration of the particle by component.
     *
     * @param   x   const ds_math::scalar, new constant acceleration of the particle in
     * the x-direction.
     * @param   y   const ds_math::scalar, new constant acceleration of the particle in
     * the y-direction.
     * @param   z   const ds_math::scalar, new constant acceleration of the particle in
     * the z-direction.
     */
    void setAcceleration(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

    /**
     * Gets the acceleration of the particle.
     *
     * @return   const ds_math::Vector3 &, acceleration of the particle in world
     * local space.
     */
    const ds_math::Vector3 &getAcceleration() const;

    /**
     * Integrates the particle forward in time by the given amount. This
     * function uses a Newton-Euler integration method, which is a linear
     * approximation of the correct integral. For this reason it may be
     * inaccurate in some cases.
     *
     * @param   duration   ds_math::scalar, period of time to integrate over. Must not be
     * 0.
     */
    void integrate(ds_math::scalar duration);

    /**
     * Clears the forces applied to the particle. This will be called
     * automatically after each integration step.
     */
    void clearAccumulator();

    /**
     * Adds the given force to the particle, to be applied at the next iteration
     * only.
     *
     * @param   force   const ds_math::Vector3 &, the force to apply.
     */
    void addForce(const ds_math::Vector3 &force);

protected:
    /** Holds the linear position of the particle in world space. */
    ds_math::Vector3 m_position;

    /** Holds the linear velocity of the particle in world space. */
    ds_math::Vector3 m_velocity;

    /**
     * Holds the acceleration of the particle. This value can be used to set
     * acceleration due to gravity (its primary use) or any other constant
     * acceleration.
     */
    ds_math::Vector3 m_acceleration;

    /**
     * Holds the amount of damping applied to linear motion. Damping is required
     * to remove energy added through numerical instability in the integrator.
     */
    ds_math::scalar m_damping;

    /**
     * Holds the inverse of the mass of the particle. It is more useful to hold
     * the inverse mass because integration is simpler and because in real-time
     * simulation it is more useful to have objects with infinite mass
     * (immovable) than zero mass (completely unstable in numberical
     * simulation).
     */
    ds_math::scalar m_inverseMass;

    /**
     * Holds the accumulated force to be applied at the next simulation
     * iteration only. This value is zeroed at each integration step.
     */
    ds_math::Vector3 m_forceAccum;

private:
};
}
