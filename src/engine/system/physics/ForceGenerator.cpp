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
#include <algorithm>

#include "math/Vector3.h"

#include "engine/system/physics/ForceGenerator.h"

namespace ds_phys
{
Gravity::Gravity(const ds_math::Vector3 &gravity)
{
    m_gravity = gravity;
}

void Gravity::updateForce(RigidBody *body, ds_math::scalar duration)
{
    // If body has infinite mass, do not apply gravity
    if (body->hasFiniteMass())
    {
        // Apply mass-scaled force to body
        body->addForce(m_gravity * body->getMass());
        std::cout << "vel: " << body->getVelocity() << std::endl;
        std::cout << "accel: " << body->getAcceleration() << std::endl;
        std::cout << "lf. accel: " << body->getLastFrameAcceleration() << std::endl;
    }
}

void ForceRegistry::add(RigidBody *body, IForceGenerator *fg)
{
    ForceRegistration reg;
    reg.body = body;
    reg.fg = fg;

    m_registrations.push_back(reg);
}

void ForceRegistry::remove(RigidBody *body, IForceGenerator *fg)
{
    ForceRegistration reg;
    reg.body = body;
    reg.fg = fg;

    Registry::iterator it =
        std::find_if(m_registrations.begin(), m_registrations.end(),
                     [&](const ForceRegistration &regA) -> bool
                     {
                         if (regA.body == reg.body && regA.fg == reg.fg)
                         {
                             return true;
                         }
                         else
                         {
                             return false;
                         }
                     });

    // TODO: Leaves gaps
    if (it != m_registrations.end())
    {
        m_registrations.erase(it);
    }
}

void ForceRegistry::clear()
{
    m_registrations.clear();
}

void ForceRegistry::updateForces(ds_math::scalar duration)
{
    std::for_each(m_registrations.begin(), m_registrations.end(),
             [&](const ForceRegistration &reg)
             {
                 reg.fg->updateForce(reg.body, duration);
             });
}
}
