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
#include <cassert>

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
    }
}

bool Gravity::isDone() const
{
    return false;
}

void Gravity::setGravity(const ds_math::Vector3 &gravity)
{
    m_gravity = gravity;
}

const ds_math::Vector3 &Gravity::getGravity() const
{
    return m_gravity;
}

ImpulseGenerator::ImpulseGenerator() : m_isDone(false)
{
}

void ImpulseGenerator::updateForce(RigidBody *body, ds_math::scalar duration)
{
    // Apply impulse
    switch (m_impulse.coordinateSpace)
    {
    case PointCoordinateSpace::None:
        body->addForce(m_impulse.force);
        break;
    case PointCoordinateSpace::World:
        body->addForceAtPoint(m_impulse.force, m_impulse.point);
        break;
    case PointCoordinateSpace::Local:
        body->addForceAtBodyPoint(m_impulse.force, m_impulse.point);
        break;
    default:
        body->addForce(m_impulse.force);
        break;
    }

    // Now that force has been applied, done
    m_isDone = true;
}

bool ImpulseGenerator::isDone() const
{
    return m_isDone;
}

void ImpulseGenerator::addImpulse(const ds_math::Vector3 &force)
{
    ImpulseInfo info;
    info.coordinateSpace = PointCoordinateSpace::None;
    info.force = force;
    info.point = ds_math::Vector3(0.0f, 0.0f, 0.0f);

    m_impulse = info;
}

void ImpulseGenerator::addImpulseAtPoint(const ds_math::Vector3 &force,
                                         const ds_math::Vector3 &point)
{
    ImpulseInfo info;
    info.coordinateSpace = PointCoordinateSpace::World;
    info.force = force;
    info.point = point;

    m_impulse = info;
}

void ImpulseGenerator::addImpulseAtBodyPoint(const ds_math::Vector3 &force,
                                             const ds_math::Vector3 &point)
{
    ImpulseInfo info;
    info.coordinateSpace = PointCoordinateSpace::Local;
    info.force = force;
    info.point = point;

    m_impulse = info;
}

void ForceRegistry::add(RigidBody *body,
                        const std::shared_ptr<IForceGenerator> &fg)
{
    ForceRegistration reg;
    reg.body = body;
    reg.fg = fg;

    m_registrations.push_back(reg);
}

void ForceRegistry::remove(RigidBody *body,
                           const std::shared_ptr<IForceGenerator> &fg)
{
    ForceRegistration reg;
    reg.body = body;
    reg.fg = fg;

    Registry::iterator it =
        std::find_if(m_registrations.begin(), m_registrations.end(),
                     [&](const ForceRegistration &regA) -> bool {
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

void ForceRegistry::removeUnused()
{
    for (unsigned int i = 0; i < m_registrations.size(); ++i)
    {
        // Remove force generators which are finished
        if (m_registrations[i].fg->isDone())
        {
            // Swap this element with last to prevent holes
            m_registrations[i] = m_registrations[m_registrations.size() - 1];
            // Remove last element (which has now been copied)
            m_registrations.pop_back();
        }
    }
}

void ForceRegistry::updateForces(ds_math::scalar duration)
{
    // Clear unused force registrations
    removeUnused();

    // For each force registration
    std::for_each(m_registrations.begin(), m_registrations.end(),
                  [&](const ForceRegistration &reg) {
                      // Apply forces from force generator (fg) to the rigid
                      // body
                      reg.fg->updateForce(reg.body, duration);
                  });
}
}
