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

#include "math/Precision.h"
#include "math/Vector3.h"

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
    Gravity(const ds_math::Vector3 &gravity);

    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

private:
    /// Acceleration due to gravity
    ds_math::Vector3 m_gravity;
};

class ImpulseGenerator : public IForceGenerator
{
public:
    /**
     * @copydoc IForceGenerator
     */
    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

    /**
     * Apply a force to the given rigidbody for a single frame (impulse). The
     * force is applied to the centre of mass of the rigid body. The force is
     * expressed in world coordinates.
     *
     * @param   body    RigidBody *, rigid body to add force to.
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     */
    void addImpulse(RigidBody *body, const ds_math::Vector3 &force);

    /**
     * Apply a force to the given rigid body for a single frame (impulse) at the
     * given point in world-space. The force is also expressed in world-space
     * coordinates.
     *
     * @param   body    RigidBody *, rigid body to add force to.
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     * @param   point   const ds_math::Vector3 &, point (in world-space
     * coordinates) to apply the force.
     */
    void addImpulseAtPoint(RigidBody *body,
                           const ds_math::Vector3 &force,
                           const ds_math::Vector3 &point);

    /**
     * Apply a force to the given rigid body for a single frame (impulse) at the
     * given point relative to the rigid bodys coordinate space. The direction
     * of the force is given in world coordinated.
     *
     * @param   body    RigidBody *, rigid body to add force to.
     * @param   force   const ds_math::Vector3 &, force to apply to the rigid
     * body (epxressed in world-space coordinates).
     * @param   point   const ds_math::Vector3 &, point (relative to the rigid
     * bodys coordinate space) to apply the force.
     */
    void addImpulseAtBodyPoint(RigidBody *body,
                               const ds_math::Vector3 &force,
                               const ds_math::Vector3 &point);

private:
    struct ImpulseInfo
    {
        /** Forces in world space coordinates */
        ds_math::Vector3 force;
        /** Points in world space coordinates */
        ds_math::Vector3 point;
    };

    struct ImpulseRegistration
    {
        RigidBody *body;
        ImpulseInfo impulse;
    };

    std::vector<ImpulseRegistration> m_impulses;
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
