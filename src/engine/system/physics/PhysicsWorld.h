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

#include "engine/system/physics/CollisionFine.h"
#include "engine/system/physics/ContactResolver.h"
#include "engine/system/physics/Contacts.h"
#include "engine/system/physics/ForceGenerator.h"
#include "engine/system/physics/RigidBody.h"
#include "math/Precision.h"
#include <map>
#include <memory>
#include <vector>

namespace ds_phys
{
typedef int CollisionPrimitiveID;

/**
 * Physics world class.
 */
class PhysicsWorld
{
public:
    /**
     * Physics world constructor.
     *
     * @param   maxContacts   unsigned int, maximum number of contacts to
     * process.
     * @param   iterations    unsigned int, maximum number of iterations to
     * perform.
     */
    PhysicsWorld(unsigned int maxContacts, unsigned int iterations = 0);
    /**
     * Physics world destructor.
     */
    ~PhysicsWorld();

    /**
     * Generate contacts for this frame.
     *
     * @return   unsigned int, number of contacts generated.
     */
    unsigned int generateContacts();

    /**
     * Perform frame initialization.
     */
    void startFrame();

    /**
     * Step the simulation forward one timestep.
     *
     * @param   duration   ds_math::scalar, duration of the timestep to
     * integrate the simulation over.
     */
    void stepSimulation(ds_math::scalar duration);

    /**
     * Add a rigid body to the simulation.
     *
     * @param   rigidBody   RigidBody *, rigid body to add to the simulation.
     */
    void addRigidBody(RigidBody *rigidBody);

    /**
     * Remove a rigid body to the simulation.
     *
     * @param   rigidBody   RigidBody *, rigid body to remove to the simulation.
     */
    void removeRigidBody(RigidBody *rigidBody);

    /**
     * Add a force generator to the given rigid body.
     *
     * @param   rigidBody        RigidBody *, rigid body to add force generator
     * to.
     * @param   forceGenerator   std::shared_ptr<IForceGenerator>, force
     * generator to add to the given rigid body.
     */
    void addForceGenerator(RigidBody *rigidBody,
                           std::shared_ptr<IForceGenerator> forceGenerator);

    /**
     * Remove a force generator to the given rigid body.
     *
     * @param   rigidBody        RigidBody *, rigid body to remove force
     * generator from.
     * @param   forceGenerator   std::shared_ptr<IForceGenerator>, force
     * generator to remove from the given rigid body.
     */
    void removeForceGenerator(RigidBody *rigidBody,
                              std::shared_ptr<IForceGenerator> forceGenerator);

    /**
     * Add a collision primitive to the physics world.
     *
     * @param   primitive   std::unique_ptr<CollisionPrimitive> &&, pointer to
     * collision primitive to add.
     * @return              CollisionPrimitiveID, id of collision primitive
     * added.
     */
    CollisionPrimitiveID
    addCollisionPrimitive(std::unique_ptr<CollisionPrimitive> &&primitive);
    /**
     * Get the collision primitive associated with the given primitive id.
     *
     * @param   id   CollisionPrimitiveID, id of collision primitive to get.
     * @return       CollisionPrimitive *, pointer to collision primitive with
     * the given id or nullptr if no collision primitive with that id.
     */
    CollisionPrimitive *getCollisionPrimitive(CollisionPrimitiveID id);
    /**
     * Get the collision primitive id of the given collision primitive.
     *
     * @param   primitive   CollisionPrimitive *, pointer to collision primitive
     * to get the collision primitive id of.
     * @return              CollisionPrimitiveID, id of the given collision
     * primitive.
     */
    CollisionPrimitiveID getCollisionPrimitiveID(CollisionPrimitive *primitive);
    /**
     * Remove a collision primitive from the physics world.
     *
     * @param   id   CollisionPrimitiveID, id of the collision primitive to
     * remove from the physics world.
     * @return       std::unique_ptr<CollisionPrimitive>, collision primitive
     * that was removed.
     */
    std::unique_ptr<CollisionPrimitive>
    removeCollisionPrimitive(CollisionPrimitiveID id);
    /**
     * Remove a collision primitive from the physics world.
     *
     * @param   primitive   CollisionPrimitive *, pointer to collision primitive
     * to remove from the physics world.
     * @return              std::unique_ptr<CollisionPrimitive>, collision
     * primitive that was removed.
     */
    std::unique_ptr<CollisionPrimitive>
    removeCollisionPrimitive(CollisionPrimitive *primitive);

    // TODO Not public
    std::vector<RigidBody *> m_rigidBodies;

    CollisionPrimitiveID m_currentCPID;
    std::map<CollisionPrimitiveID, std::unique_ptr<CollisionPrimitive>>
        m_collisionPrimitives;

    ContactResolver m_contactResolver;

private:
    // Use Bullet to generate contacts
    // btCollisionDispatcher *m_dispatcher;
    // btCollisionWorld *m_collisionWorld;
    // btDefaultCollisionConfiguration *m_collisionConfiguration;
    // btBroadphasInterface *m_broadPhase;
    // std::vector<CollisionPrimitive *> m_collisionBodies;

    /** Holds the maximum number of contacts. */
    const static unsigned MAX_CONTACTS = 256;

    /** Holds the array of contacts. */
    Contact m_contacts[MAX_CONTACTS];

    /** Collision generation structure */
    CollisionData m_collisionData;

    // Use own physics to resolve them
    // ContactResolver m_contactResolver;

    ForceRegistry m_forceRegistry;
};
}
