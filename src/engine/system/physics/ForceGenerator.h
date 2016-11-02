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
    ImpulseGenerator(const ds_math::Vector3 &force);

    virtual void updateForce(RigidBody *body, ds_math::scalar duration);

private:
    ds_math::Vector3 m_force;
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
