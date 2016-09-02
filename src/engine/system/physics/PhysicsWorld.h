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
 * Modified by Samuel Evans-Powell.
 *
 * @author Ian Millington
 * @author Samuel Evans-Powell
 */
#pragma once

#include "math/Precision.h"

#include "engine/system/physics/RigidBody.h"
#include "engine/system/physics/ForceGenerator.h"
#include "engine/system/physics/Contacts.h"

namespace ds_phys
{
class PhysicsWorld
{
public:
    PhysicsWorld(unsigned int maxContacts, unsigned int iterations = 0);
    ~PhysicsWorld();

    unsigned int generateContacts();

    void startFrame();

    void stepSimulation(ds_math::scalar duration);

    void addRigidBody(RigidBody *rigidBody);

    void removeRigidBody(RigidBody *rigidBody);

    void addForceGenerator(RigidBody *rigidBody,
                           std::shared_ptr<IForceGenerator> forceGenerator);

    void removeForceGenerator(RigidBody *rigidBody,
                              std::shared_ptr<IForceGenerator> forceGenerator);

    std::vector<RigidBody *> m_rigidBodies;

private:
    // Use Bullet to generate contacts
    // btCollisionDispatcher *m_dispatcher;
    // btCollisionWorld *m_collisionWorld;
    // btDefaultCollisionConfiguration *m_collisionConfiguration;
    // btBroadphasInterface *m_broadPhase;

    // Use own physics to resolve them
    ContactResolver m_contactResolver;

    // TODO: Use std::list instead?

    ForceRegistry m_forceRegistry;
};
}
