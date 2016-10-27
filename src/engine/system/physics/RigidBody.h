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
 * @author Samuel Evans-Powell
 * @author Julian Smith
 * @author Luke Phipps
 */
#pragma once

#include "math/Matrix3.h"
#include "math/Matrix4.h"
#include "math/Precision.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"
#include <vector>

namespace ds_phys
{
class CollisionPrimitive;

class RigidBody
{
public:
    RigidBody();

    /**
     * @name Integration and Simulation Functions
     *
     * These functions are used to simulate the rigid body's
     * motion over time. A normal application sets up one or more
     * rigid bodies, applies permanent forces (i.e. gravity), then
     * adds transient forces each frame, and integrates, prior to
     * rendering.
     *
     * Currently the only integration function provided is the
     * first order Newton Euler method.
     */
    /*@{*/

    ///
    /// Calculates internal data from state data.
    /// Called after the body's state is altered directly.
    /// Ommit this step if you change the body's state and intend on integrating
    /// before querying any data.
    ///
    void calculateDerivedData();

    ///
    /// Integrates the rigid body forward in time by the given duration.
    /// Uses a Newton-Euler integration method - linear approximation.
    /// May be innacurate in some cases.
    ///
    void integrate(ds_math::scalar duration);

    /*@}*/


    /**
     * @name Accessor Functions for the Rigid Body's State
     *
     * These functions provide access to the rigid body's
     * characteristics or state. These data can be accessed
     * individually, or en masse as an array of values
     * (e.g. getCharacteristics, getState). When setting new data,
     * make sure the calculateInternals function, or an
     * integration routine, is called before trying to get data
     * from the body, since the class contains a number of
     * dependent values that will need recalculating.
     */
    /*@{*/

    ///
    /// Sets the mass of the rigid body.
    ///
    /// @param mass The new mass of the body. May not be zero. Don't use small
    /// numbers.
    ///
    /// @warning Invalidates internal data for the rigid body. Call integration
    /// function or calculateInternals before getting settings.
    ///
    void setMass(const ds_math::scalar mass);

    ///
    /// Gets the mass of the rigid body.
    ///
    /// @return The current mass of the rigid body.
    ///
    ds_math::scalar getMass() const;

    ///
    /// Set the inverse mass of the rigid body.
    ///
    /// @param inverseMass The new inverse mass of the body. Zero for an
    /// infinite mass.
    ///
    /// @warning Invalidates internal data for the rigid body. Call integration
    /// function or calculateInternals before getting settings.
    ///
    void setInverseMass(const ds_math::scalar inverseMass);

    ///
    /// Gets the inverse masss of the rigid body.
    ///
    /// @return The current inverse mass of the rigid body.
    ///
    ds_math::scalar getInverseMass() const;

    ///
    /// Returns true if the mass of the body is not-infinite.
    ///
    bool hasFiniteMass() const;

    ///
    /// Sets the inertia tensor for the rigid body.
    ///
    /// @param inertiaTensor The inertia tensor for the rigid body. Must be a
    /// full rank matrix and must be invertible.
    ///
    /// @warning Invalidates internal data for the rigid body. Call integration
    /// function or calculateInternals before getting settings.
    ///
    void setInertiaTensor(const ds_math::Matrix3 &inertiaTensor);

    ///
    /// Set the inertia tensor for the rigid body from the inertia products.
    ///
    /// @param   inertiaTensor   const ds_math::Vector3 &, the inertia tensor
    /// products for the rigid body.
    ///
    /// @warning Invalidates internal data for the rigid body. Call integration
    /// function or calculateInternals before getting settings.
    ///
    void setInertiaTensor(const ds_math::Vector3 &inertiaTensor);

    ///
    /// Gets the inverse mass of the rigid body
    ///
    /// @return The current inverse mass of the rigid body
    ///
    void getInertiaTensor(ds_math::Matrix3 *inertiaTensor) const;

    ///
    /// Gets a new copy of the current inertia tensor of the rigid body
    ///
    /// @return matrix containing the current inertia tensor
    ///         the inertia tensor is expressed in the rigid body's local space
    ///
    ds_math::Matrix3 getInertiaTensor() const;

    ///
    /// Copies the current inertia tensor of the rigid body into given matrix
    ///
    /// @param inertiaTensor is a pointer to a matrix to hold the current
    /// inertia tensor in rigid body
    ///        the inertia tensor is expressed in word space
    ///
    ///
    void getInertiaTensorWorld(ds_math::Matrix3 *inertiaTensor) const;

    ///
    /// Gets a copy of the current inertia tensor of the rigid body
    ///
    /// @return new matrix containing the current inertia tensor
    ///         inertia tensor is expressed in world space
    ///
    ///
    ds_math::Matrix3 getInertiaTensorWorld() const;

    ///
    /// Sets the inverse inertia tensor for the rigid body
    ///
    /// @param inverseInertiaTensor The inverse inertia tensor for the rigid
    /// body
    ///        This must be a full rank matrix and must be invertible
    ///
    /// @warning Invalidates internal data for the rigid body.
    /// Call integration function or calculateInternals before getting settings.
    ///
    void setInverseInertiaTensor(const ds_math::Matrix3 &inverseInertiaTensor);

    ///
    /// Set the inverse inertia tensor for the rigid body from the inertia
    /// products.
    ///
    /// @param   invInertiaTensor   const ds_math::Vector3 &, the inverse
    /// inertia tensor products for the rigid body.
    ///
    /// @warning Invalidates internal data for the rigid body. Call integration
    /// function or calculateInternals before getting settings.
    ///
    void setInverseInertiaTensor(const ds_math::Vector3 &invInertiaTensor);

    ///
    /// Copies the current inverse inertia tensor of the rigid body into matrix
    /// given
    ///
    /// @param inverseInertiaTensor A pointer to a matrix to hold the current
    ///        ivnerse inertia tensor of the rigid body
    ///        The inertia tensor is expressed in the rigid body local space
    ///
    void getInverseInertiaTensor(ds_math::Matrix3 *inverseInertiaTensor) const;

    ///
    /// Gets a copy of the current inverse inertia tensor of the rigid body
    ///
    /// @return A new matrix containing the current inverse inertia tensor
    ///         The inertia tensor is expressed in the rigid body local space
    ///
    ds_math::Matrix3 getInverseInertiaTensor() const;

    ///
    /// Copies the inverse inertia tensor of the rigid body into the given
    /// matrix
    ///
    /// @param inverseInertiaTensor A pointer to a matrix to hold the current
    ///        ivnerse inertia tensor of the rigid body
    ///        The inertia tensor is expressed in the rigid body local space
    ///
    void
    getInverseInertiaTensorWorld(ds_math::Matrix3 *inverseInertiaTensor) const;

    ///
    /// Gets a copy of the current inverse inertia tensor of the rigid body
    ///
    /// @return A new matrix cotnaining the current inverse inertia tensor
    ///         The inertia tensor is expressed in world space
    ///
    ///
    ds_math::Matrix3 getInverseInertiaTensorWorld() const;

    ///
    /// Sets both linear and angular damping in one function call
    ///
    /// @param linearDamping The speed that velocity is shed from the rigid body
    ///
    /// @param angularDamping The speed that rotation is shed from the rigid
    /// body
    ///
    /// @see setLinearDamping
    /// @see setAngularDamping
    ///
    void setDamping(const ds_math::scalar linearDamping,
                    const ds_math::scalar angularDamping);

    ///
    /// Sets the linear damping for the rigid body
    ///
    /// @param linearDamping The speed that velocity is shed from the rigid body
    ///
    /// @see setAngularDamping
    ///
    void setLinearDamping(const ds_math::scalar linearDamping);

    ///
    /// Gets the current linear damping value
    ///
    /// @return The current linear damping value
    ///
    ds_math::scalar getLinearDamping() const;

    ///
    /// Sets the angular damping for the rigid body
    ///
    /// @param angularDamping The speed that rotation is shed from the rigid
    /// body
    ///
    /// @see setLinearDamping
    ///
    void setAngularDamping(const ds_math::scalar angularDamping);

    ///
    /// Gets the current angular damping value
    ///
    /// @return The current angular damping value
    ///
    ds_math::scalar getAngularDamping() const;

    ///
    /// Sets the position of the rigid body
    ///
    /// @param position The new position of the rigid body
    ///
    void setPosition(const ds_math::Vector3 &position);

    ///
    /// Sets the position of the rigid body by component
    ///
    /// @param x The x coordinate of the new position
    ///
    /// @param y The y coordinate of the new position
    ///
    /// @param z The z coordinate of the new position
    ///
    void setPosition(const ds_math::scalar x,
                     const ds_math::scalar y,
                     const ds_math::scalar z);

    ///
    /// Fills the given vector with the position of the rigid body.
    ///
    /// @param position A pointer to a vector into which to write
    /// the position.
    ///
    void getPosition(ds_math::Vector3 *position) const;

    ///
    /// Gets the position of the rigid body.
    ///
    /// @return The position of the rigid body.
    ///
    ds_math::Vector3 getPosition() const;

    ///
    /// Sets the orientation of the rigid body.
    ///
    /// @param orientation The new orientation of the rigid body.
    ///
    /// @note The given orientation does not need to be normalised,
    /// and can be zero. This function automatically constructs a
    /// valid rotation quaternion with (0,0,0,0) mapping to
    /// (1,0,0,0).
    ///
    void setOrientation(const ds_math::Quaternion &orientation);

    ///
    /// Sets the orientation of the rigid body by component.
    ///
    /// @param r The real component of the rigid body's orientation
    /// quaternion.
    ///
    /// @param i The first complex component of the rigid body's
    /// orientation quaternion.
    ///
    /// @param j The second complex component of the rigid body's
    /// orientation quaternion.
    ///
    /// @param k The third complex component of the rigid body's
    /// orientation quaternion.
    ///
    /// @note The given orientation does not need to be normalised,
    /// and can be zero. This function automatically constructs a
    /// valid rotation quaternion with (0,0,0,0) mapping to
    /// (1,0,0,0).
    ///
    void setOrientation(const ds_math::scalar r,
                        const ds_math::scalar i,
                        const ds_math::scalar j,
                        const ds_math::scalar k);

    ///
    /// Fills the given quaternion with the current value of the
    /// rigid body's orientation.
    ///
    /// @param orientation A pointer to a quaternion to receive the
    /// orientation data.
    ///
    void getOrientation(ds_math::Quaternion *orientation) const;

    ///
    /// Gets the orientation of the rigid body.
    ///
    /// @return The orientation of the rigid body.
    ///
    ds_math::Quaternion getOrientation() const;

    ///
    /// Fills the given matrix with a transformation representing
    /// the rigid body's orientation.
    ///
    /// @note Transforming a direction vector by this matrix turns
    /// it from the body's local space to world space.
    ///
    /// @param matrix A pointer to the matrix to fill.
    ///
    void getOrientation(ds_math::Matrix3 *matrix) const;

    ///
    /// Fills the given matrix data structure with a transformation
    /// representing the rigid body's orientation.
    ///
    /// @note Transforming a direction vector by this matrix turns
    /// it from the body's local space to world space.
    ///
    /// @param matrix A pointer to the matrix to fill.
    ///
    void getOrientation(ds_math::scalar matrix[9]) const;

    ///
    /// Fills the given matrix with a transformation representing
    /// the rigid body's position and orientation.
    ///
    /// @note Transforming a vector by this matrix turns it from
    /// the body's local space to world space.
    ///
    /// @param transform A pointer to the matrix to fill.
    ///
    void getTransform(ds_math::Matrix4 *transform) const;

    ///
    /// Fills the given matrix data structure with a
    /// transformation representing the rigid body's position and
    /// orientation.
    ///
    /// @note Transforming a vector by this matrix turns it from
    /// the body's local space to world space.
    ///
    /// @param matrix A pointer to the matrix to fill.
    ///
    void getTransform(ds_math::scalar matrix[16]) const;

    ///
    /// Fills the given matrix data structure with a
    /// transformation representing the rigid body's position and
    /// orientation. The matrix is transposed from that returned
    /// by getTransform. This call returns a matrix suitable
    /// for applying as an OpenGL transform.
    ///
    /// @note Transforming a vector by this matrix turns it from
    /// the body's local space to world space.
    ///
    /// @param matrix A pointer to the matrix to fill.
    ///
    void getGLTransform(float matrix[16]) const;

    ///
    /// Gets a transformation representing the rigid body's
    /// position and orientation.
    ///
    /// @note Transforming a vector by this matrix turns it from
    /// the body's local space to world space.
    ///
    /// @return The transform matrix for the rigid body.
    ///
    ds_math::Matrix4 getTransform() const;

    ///
    /// Converts the given point from world space into the body's
    /// local space.
    ///
    /// @param point The point to covert, given in world space.
    ///
    /// @return The converted point, in local space.
    ///
    ds_math::Vector3 getPointInLocalSpace(const ds_math::Vector3 &point) const;

    ///
    /// Converts the given point from world space into the body's
    /// local space.
    ///
    /// @param point The point to covert, given in local space.
    ///
    /// @return The converted point, in world space.
    ///
    ds_math::Vector3 getPointInWorldSpace(const ds_math::Vector3 &point) const;

    ///
    /// Converts the given direction from world space into the
    /// body's local space.
    ///
    /// @note When a direction is converted between frames of
    /// reference, there is no translation required.
    ///
    /// @param direction The direction to covert, given in world
    /// space.
    ///
    /// @return The converted direction, in local space.
    ///
    ds_math::Vector3
    getDirectionInLocalSpace(const ds_math::Vector3 &direction) const;

    ///
    /// Converts the given direction from world space into the
    /// body's local space.
    ///
    /// @note When a direction is converted between frames of
    /// reference, there is no translation required.
    ///
    /// @param direction The direction to covert, given in local
    /// space.
    ///
    /// @return The converted direction, in world space.
    ///
    ds_math::Vector3
    getDirectionInWorldSpace(const ds_math::Vector3 &direction) const;

    ///
    /// Sets the velocity of the rigid body.
    ///
    /// @param velocity The new velocity of the rigid body. The
    /// velocity is given in world space.
    ///
    void setVelocity(const ds_math::Vector3 &velocity);

    ///
    /// Sets the velocity of the rigid body by component. The
    /// velocity is given in world space.
    ///
    /// @param x The x coordinate of the new velocity of the rigid
    /// body.
    ///
    /// @param y The y coordinate of the new velocity of the rigid
    /// body.
    ///
    /// @param z The z coordinate of the new velocity of the rigid
    /// body.
    ///
    void setVelocity(const ds_math::scalar x,
                     const ds_math::scalar y,
                     const ds_math::scalar z);

    ///
    /// Fills the given vector with the velocity of the rigid body.
    ///
    /// @param velocity A pointer to a vector into which to write
    /// the velocity. The velocity is given in world local space.
    ///
    void getVelocity(ds_math::Vector3 *velocity) const;

    ///
    /// Gets the velocity of the rigid body.
    ///
    /// @return The velocity of the rigid body. The velocity is
    /// given in world local space.
    ///
    ds_math::Vector3 getVelocity() const;

    ///
    /// Applies the given change in velocity.
    ///
    void addVelocity(const ds_math::Vector3 &deltaVelocity);

    ///
    /// Sets the rotation of the rigid body.
    ///
    /// @param rotation The new rotation of the rigid body. The
    /// rotation is given in world space.
    ///
    void setRotation(const ds_math::Vector3 &rotation);

    ///
    /// Sets the rotation of the rigid body by component. The
    /// rotation is given in world space.
    ///
    /// @param x The x coordinate of the new rotation of the rigid
    /// body.
    ///
    /// @param y The y coordinate of the new rotation of the rigid
    /// body.
    ///
    /// @param z The z coordinate of the new rotation of the rigid
    /// body.
    ///
    void setRotation(const ds_math::scalar x,
                     const ds_math::scalar y,
                     const ds_math::scalar z);

    ///
    /// Fills the given vector with the rotation of the rigid body.
    ///
    /// @param rotation A pointer to a vector into which to write
    /// the rotation. The rotation is given in world local space.
    ///
    void getRotation(ds_math::Vector3 *rotation) const;

    ///
    /// Gets the rotation of the rigid body.
    ///
    /// @return The rotation of the rigid body. The rotation is
    /// given in world local space.
    ///
    ds_math::Vector3 getRotation() const;

    ///
    /// Applies the given change in rotation.
    ///
    void addRotation(const ds_math::Vector3 &deltaRotation);

    ///
    /// Returns true if the body is awake and responding to
    /// integration.
    ///
    /// @return The awake state of the body.
    ///
    bool getAwake() const
    {
        return m_isAwake;
    }

    ///
    /// Sets the awake state of the body. If the body is set to be
    /// not awake, then its velocities are also cancelled, since
    /// a moving body that is not awake can cause problems in the
    /// simulation.
    ///
    /// @param awake The new awake state of the body.
    ///
    void setAwake(const bool awake = true);

    ///
    /// Returns true if the body is allowed to go to sleep at
    /// any time.
    ///
    bool getCanSleep() const
    {
        return m_canSleep;
    }

    ///
    /// Sets whether the body is ever allowed to go to sleep. Bodies
    /// under the player's control, or for which the set of
    /// transient forces applied each frame are not predictable,
    /// should be kept awake.
    ///
    /// @param canSleep Whether the body can now be put to sleep.
    ///
    void setCanSleep(const bool canSleep = true);

    /*@}*/


    /**
    * @name Retrieval Functions for Dynamic Quantities
    *
    * These functions provide access to the acceleration
    * properties of the body. The acceleration is generated by
    * the simulation from the forces and torques applied to the
    * rigid body. Acceleration cannot be directly influenced, it
    * is set during integration, and represent the acceleration
    * experienced by the body of the previous simulation step.
    */
    /*@{*/

    ///
    /// Fills the given vector with the current accumulated value
    /// for linear acceleration. The acceleration accumulators
    /// are set during the integration step. They can be read to
    /// determine the rigid body's acceleration over the last
    /// integration step. The linear acceleration is given in world
    /// space.
    ///
    /// @param linearAcceleration A pointer to a vector to receive
    /// the linear acceleration data.
    ///
    void getLastFrameAcceleration(ds_math::Vector3 *linearAcceleration) const;

    ///
    /// Gets the current accumulated value for linear
    /// acceleration. The acceleration accumulators are set during
    /// the integration step. They can be read to determine the
    /// rigid body's acceleration over the last integration
    /// step. The linear acceleration is given in world space.
    ///
    /// @return The rigid body's linear acceleration.
    ///
    ds_math::Vector3 getLastFrameAcceleration() const;

    /*@}*/


    ///
    /// @name Force, Torque and Acceleration Set-up Functions
    ///
    /// These functions set up forces and torques to apply to the
    /// rigid body.
    ///
    /*@{*/

    ///
    /// Clears the forces and torques in the accumulators. This will
    /// be called automatically after each intergration step.
    ///
    void clearAccumulators();

    ///
    /// Adds the given force to centre of mass of the rigid body.
    /// The force is expressed in world-coordinates.
    ///
    /// @param force The force to apply.
    ///
    void addForce(const ds_math::Vector3 &force);

    ///
    /// Adds the given force to the given point on the rigid body.
    /// Both the force and the
    /// application point are given in world space. Because the
    /// force is not applied at the centre of mass, it may be split
    /// into both a force and torque.
    ///
    /// @param force The force to apply.
    ///
    /// @param point The location at which to apply the force, in
    /// world-coordinates.
    ///
    void addForceAtPoint(const ds_math::Vector3 &force,
                         const ds_math::Vector3 &point);

    ///
    /// Adds the given force to the given point on the rigid body.
    /// The direction of the force is given in world coordinates,
    /// but the application point is given in body space. This is
    /// useful for spring forces, or other forces fixed to the
    /// body.
    ///
    /// @param force The force to apply.
    ///
    /// @param point The location at which to apply the force, in
    /// body-coordinates.
    ///
    void addForceAtBodyPoint(const ds_math::Vector3 &force,
                             const ds_math::Vector3 &point);

    ///
    /// Adds the given torque to the rigid body.
    /// The force is expressed in world-coordinates.
    ///
    /// @param torque The torque to apply.
    ///
    void addTorque(const ds_math::Vector3 &torque);

    ///
    /// Sets the constant acceleration of the rigid body.
    ///
    /// @param acceleration The new acceleration of the rigid body.
    ///
    void setAcceleration(const ds_math::Vector3 &acceleration);

    ///
    /// Sets the constant acceleration of the rigid body by component.
    ///
    /// @param x The x coordinate of the new acceleration of the rigid
    /// body.
    ///
    /// @param y The y coordinate of the new acceleration of the rigid
    /// body.
    ///
    /// @param z The z coordinate of the new acceleration of the rigid
    /// body.
    ///
    void setAcceleration(const ds_math::scalar x,
                         const ds_math::scalar y,
                         const ds_math::scalar z);

    ///
    /// Fills the given vector with the acceleration of the rigid body.
    ///
    /// @param acceleration A pointer to a vector into which to write
    /// the acceleration. The acceleration is given in world local space.
    ///
    void getAcceleration(ds_math::Vector3 *acceleration) const;

    ///
    /// Gets the acceleration of the rigid body.
    ///
    /// @return The acceleration of the rigid body. The acceleration is
    /// given in world local space.
    ///
    ds_math::Vector3 getAcceleration() const;

    void addCollisionPrimitive(CollisionPrimitive *prim);
    void removeCollisionPrimitive(unsigned id);
    CollisionPrimitive *getCollisionPrimitive(unsigned id);
    unsigned getCollisionPrimitiveCount();

protected:
    std::vector<CollisionPrimitive *> m_primitives;

    /**
         * @name Characteristic Data and State
         *
         * This data holds the state of the rigid body. There are two
         * sets of data: characteristics and state.
         *
         * Characteristics are properties of the rigid body
         * independent of its current kinematic situation. This
         * includes mass, moment of inertia and damping
         * properties. Two identical rigid bodys will have the same
         * values for their characteristics.
         *
         * State includes all the characteristics and also includes
         * the kinematic situation of the rigid body in the current
         * simulation. By setting the whole state data, a rigid body's
         * exact game state can be replicated. Note that state does
         * not include any forces applied to the body. Two identical
         * rigid bodies in the same simulation will not share the same
         * state values.
         *
         * The state values make up the smallest set of independent
         * data for the rigid body. Other state data is calculated
         * from their current values. When state data is changed the
         * dependent values need to be updated: this can be achieved
         * either by integrating the simulation, or by calling the
         * calculateInternals function. This two stage process is used
         * because recalculating internals can be a costly process:
         * all state changes should be carried out at the same time,
         * allowing for a single call.
         *
         * @see calculateInternals
         */
    /*@{*/


    ///
    /// Holds inverse mass of rigid body
    /// Inverse is one over mass, makes intergration simpler
    /// More useful to have infinite mass than zero mass
    ///
    ds_math::scalar m_inverseMass;

    ///
    /// Holds inverse of rigid body inertia tensor
    /// Must not degenerate (have zero inertia along one axis)
    /// Used for similar reasons than inverse mass
    /// Is given in body space (unlike other rigid body variables)
    ///
    ds_math::Matrix3 m_inverseInertiaTensor;

    ///
    /// Holds amount of damping applied to linear motion
    /// Required to remove energy added through numerical instability to the
    /// integrator
    ///
    ///
    ///
    ds_math::scalar m_linearDamping;

    ///
    /// Holds amount of damping applied to angular motion
    /// Required to remove energy added through numerical instability to the
    /// integrator
    ///
    ///
    ///
    ds_math::scalar m_angularDamping;

    ///
    /// Holds the linear position of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_position;

    ///
    /// Holds the angular orientation of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Quaternion m_orientation;

    ///
    /// Holds the linear velocity of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_velocity;

    ///
    /// Holds the angular velocity (rotation) of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_rotation;

    /*@}*/


    /**
        * @name Derived Data
        *
        * These data members hold information that is derived from
        * the other data in the class.
        */
    /*@{*/

    ///
    /// Holds the inverse intertia tensor of the rigid body in world space
    /// Inverse intertia tensor is specified in rigid body's local space
    ///
    ///
    ///
    ds_math::Matrix3 m_inverseInertiaTensorWorld;

    ///
    /// Holds the amount  of motion of the body
    /// Recency weighted mean that can be used to put the body to sleep
    /// (Weighted towards what has recently been used)
    ///
    ///
    ds_math::scalar m_motion;

    ///
    /// Can be put to sleep to avoid being updated by integration functions
    /// or effected by collisions with the world
    ///
    ///
    ///
    bool m_isAwake;

    ///
    /// Some rigid bodies may never be asleep
    /// @Example - User controlled bodies should always be awake
    ///
    ///
    ///
    bool m_canSleep;

    ///
    /// Holds transform matrix for converting body space into world space (and
    /// vice versa)
    /// Acheived by calling the GetPointIn*Space functions
    ///
    /// @see getPointInLocalSpace
    /// @see getPointInWorldSpace
    /// @see getTransform
    ///
    ds_math::Matrix4 m_transformMatrix;

    /*@}*/


    /**
        * @name Force and Torque Accumulators
        *
        * These data members store the current force, torque and
        * acceleration of the rigid body. Forces can be added to the
        * rigid body in any order, and the class decomposes them into
        * their constituents, accumulating them for the next
        * simulation step. At the simulation step, the accelerations
        * are calculated and stored to be applied to the rigid body.
        */
    /*@{*/

    ///
    /// Holds accumulated force to be applied at the next integration step
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_forceAccum;

    ///
    /// Holds the accumulated torque to be applied at the next integration step
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_torqueAccum;

    ///
    /// Holds acceleration of the rigid body
    /// This value can be used to set acceleration due to gravity (primary use)
    /// or other acceleration
    ///
    ///
    ds_math::Vector3 m_acceleration;

    ///
    /// Holds the linear acceleration of the rigid body for previous frame
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_lastFrameAcceleration;

    /*@}*/


private:
};
}
