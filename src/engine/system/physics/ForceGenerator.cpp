#include <algorithm>

#include "math/Vector3.h"

#include "engine/system/physics/ForceGenerator.h"

namespace ds_phys
{
GoDown::GoDown(float speed) : m_speed(speed)
{
}

void GoDown::updateForce(RigidBody *body, ds_math::scalar duration)
{
    body->setPosition(body->getPosition() -
                      ds_math::Vector3(0, m_speed * duration, 0));
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
    for_each(m_registrations.begin(), m_registrations.end(),
             [&](const ForceRegistration &reg)
             {
                 reg.fg->updateForce(reg.body, duration);
             });
}
}
