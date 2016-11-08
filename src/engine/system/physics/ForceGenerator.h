/**
 *
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
 * @author Ian Millington
 * @author Samuel Evans-Powell (modified)
 */
#pragma once

#include <map>
#include <vector>
#include <memory>

#include "math/Precision.h"
#include "math/Vector3.h"

#include "engine/system/physics/RigidBody.h"

namespace ds_phys
{
class IForceGenerator
{
public:
    /**
     * Update the force applied to the given rigid body.
     *
     * @param   body       RigidBody *, body to apply forces to.
     * @param   duration   ds_math::scalar, time between frames.
     */
    virtual void updateForce(RigidBody *body, ds_math::scalar duration) = 0;

    /**
     * Returns true if the force generator has finished applying forces, false
     * otherwise.
     *
     * Note: this is used to determine if the force generator can be removed
     * from the ForceRegistry.
     *
     * @return   bool, true if force generator has finished applying forces and
     * false otherwise.
     */
    virtual bool isDone() const = 0;
};

/**
 * Apply a gravitational force to a rigid body.
 */
class Gravity : public IForceGenerator
{
public:
    /**
     * Constructor.
     *
     * @param   gravity   const ds_math::Vector3 &, gravity vector to use.
     */
    Gravity(const ds_math::Vector3 &gravity);

    /**
     * Applies gravity force to the given rigid body.
     *
     * @copydoc IForceGenerator::updateForce
     */
    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

    /**
     * In this case, always returns false.
     *
     * @copydoc IForceGenerator::isDone
     */
    virtual bool isDone() const;

    /**
     * Set the direction and magnitude of gravity for this force generator.
     *
     * @param   gravity   const ds_math::Vector3 &, gravity vector to apply to
     * all rigid bodies registered with this force generator.
     *
     * @param   gravity   const ds_math::Vector3 &, gravity vector to apply.
     */
    void setGravity(const ds_math::Vector3 &gravity);

    /**
     * Get the direction and magnitude of gravity for this force generator.
     *
     * @return   const ds_math::Vector3 &, gravity vector this force generator
     * uses.
     */
    const ds_math::Vector3 &getGravity() const;

private:
    /// Acceleration due to gravity
    ds_math::Vector3 m_gravity;
};

/**
 * Used to apply a force to a rigid body for a single frame.
 */
class ImpulseGenerator : public IForceGenerator
{
public:
    /**
     * Default constructor;
     */
    ImpulseGenerator();

    /**
     * Applies a force to the given rigid body for one frame.
     *
     * @copydoc IForceGenerator::updateForce
     */
    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

    /**
     * @copydoc IForceGenerator::isDone
     */
    virtual bool isDone() const;

    /**
     * Apply a force to the given rigidbody for a single frame (impulse). The
     * force is applied to the centre of mass of the rigid body. The force is
     * expressed in world coordinates.
     *
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     */
    void addImpulse(const ds_math::Vector3 &force);

    /**
     * Apply a force to the given rigid body for a single frame (impulse) at the
     * given point in world-space. The force is also expressed in world-space
     * coordinates.
     *
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     * @param   point   const ds_math::Vector3 &, point (in world-space
     * coordinates) to apply the force.
     */
    void addImpulseAtPoint(const ds_math::Vector3 &force,
                           const ds_math::Vector3 &point);

    /**
     * Apply a force to the given rigid body for a single frame (impulse) at the
     * given point relative to the rigid bodys coordinate space. The direction
     * of the force is given in world coordinated.
     *
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     * @param   point   const ds_math::Vector3 &, point (relative to the rigid
     * bodys coordinate space) to apply the force.
     */
    void addImpulseAtBodyPoint(const ds_math::Vector3 &force,
                               const ds_math::Vector3 &point);

private:
    enum class PointCoordinateSpace
    {
        None,
        World,
        Local
    };

    struct ImpulseInfo
    {
        /** Coordinate space of point */
        PointCoordinateSpace coordinateSpace;
        /** Forces in world space coordinates */
        ds_math::Vector3 force;
        /** Where to apply force */
        ds_math::Vector3 point;
    };

    ImpulseInfo m_impulse;

    bool m_isDone;
};

class ForceRegistry
{
public:
    void add(RigidBody *body, const std::shared_ptr<IForceGenerator> &fg);

    void remove(RigidBody *body, const std::shared_ptr<IForceGenerator> &fg);

    /**
     * Removes all rigid body and force generator registrations from the force
     * registry.
     *
     * Note: Does not free any memory!
     */
    void clear();

    /**
     * Allows each registered force generator to apply forces to its paired
     * rigid body. Also removes force generators that are no longer applying
     * forces.
     *
     * @param   duration   scalar, time since last frame.
     */
    void updateForces(ds_math::scalar duration);

protected:
    /**
     * Remove unused force generators from the force registry. A force generator
     * is considered unused if it's isDone() method returns true.
     */
    void removeUnused();

    struct ForceRegistration
    {
        RigidBody *body;
        std::shared_ptr<IForceGenerator> fg;
    };

    typedef std::vector<ForceRegistration> Registry;
    Registry m_registrations;

private:
};
}
