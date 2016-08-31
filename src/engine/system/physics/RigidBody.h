#pragma once

#include "math/Vector3.h"
#include "math/Precision.h"

namespace ds_phys
{
class RigidBody
{
public:
    void integrate(ds_math::scalar duration);

    void clearAccumulators();

    ds_math::Vector3 getPosition() const;

    void setPosition(const ds_math::Vector3 &position);
    
    ////////////////////////////////////////////////////////////////////////////////////////////// BEGIN COPY PASTE
    
    
     /**
         * @name Constructor and Destructor
         *
         * There are no data members in the rigid body class that are
         * created on the heap. So all data storage is handled
         * automatically.
         */
        /*@{*/

        /*@}*/


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
        /// Ommit this step if you change the body's state and intend on integrating before querying any data.
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
        /// @param mass The new mass of the body. May not be zero. Don't use small numbers.
        ///
        /// @warning Invalidates internal data for the rigid body. Call integration function or calculateInternals before getting settings.
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
        /// @param inverseMass The new inverse mass of the body. Zero for an infinite mass.
        ///
        /// @warning Invalidates internal data for the rigid body. Call integration function or calculateInternals before getting settings.
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
        /// @param inertiaTensor The inertia tensor for the rigid body. Must be a full rank matrix and must be invertible.
        ///
        /// @warning Invalidates internal data for the rigid body. Call integration function or calculateInternals before getting settings.
        /// 
        void setInertiaTensor(const ds_math::Matrix3 &inertiaTensor);

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
        /// @param inertiaTensor is a pointer to a matrix to hold the current inertia tensor in rigid body
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
        /// @param inverseInertiaTensor The inverse inertia tensor for the rigid body
        ///        This must be a full rank matrix and must be invertible
        ///
        /// @warning Invalidates internal data for the rigid body. 
        /// Call integration function or calculateInternals before getting settings.
        ///
        void setInverseInertiaTensor(const ds_math::Matrix3 &inverseInertiaTensor);

        ///
        /// Copies the current inverse inertia tensor of the rigid body into matrix given
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
        /// Copies the inverse inertia tensor of the rigid body into the given matrix
        ///
        /// @param inverseInertiaTensor A pointer to a matrix to hold the current
        ///        ivnerse inertia tensor of the rigid body
        ///        The inertia tensor is expressed in the rigid body local space       
        /// 
        void getInverseInertiaTensorWorld(ds_math::Matrix3 *inverseInertiaTensor) const;

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
        /// @param angularDamping The speed that rotation is shed from the rigid body
        ///
        /// @see setLinearDamping
        /// @see setAngularDamping
        /// 
        void setDamping(const ds_math::scalar linearDamping, const ds_math::scalar angularDamping);

        ///
        /// Sets the linear damping for the rigid body
        ///
        /// @param linearDamping The speed that velocity is shed from the rigid body
        ///
        /// @see setAngularDamping
        ///
        void setLinearDamping(const ds_math::scalar linearDamping);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::scalar getLinearDamping() const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setAngularDamping(const ds_math::scalar angularDamping);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::scalar getAngularDamping() const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setPosition(const ds_math::Vector3 &position);

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setPosition(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getPosition(ds_math::Vector3 *position) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getPosition() const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setOrientation(const ds_math::Quaternion &orientation);

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setOrientation(const ds_math::scalar r, const ds_math::scalar i,
            const ds_math::scalar j, const ds_math::scalar k);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getOrientation(ds_math::Quaternion *orientation) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Quaternion getOrientation() const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getOrientation(ds_math::Matrix3 *matrix) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getOrientation(ds_math::scalar matrix[9]) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getTransform(ds_math::Matrix4 *transform) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getTransform(ds_math::scalar matrix[16]) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getGLTransform(float matrix[16]) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Matrix4 getTransform() const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getPointInLocalSpace(const ds_math::Vector3 &point) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getPointInWorldSpace(const ds_math::Vector3 &point) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getDirectionInLocalSpace(const ds_math::Vector3 &direction) const;

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getDirectionInWorldSpace(const ds_math::Vector3 &direction) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setVelocity(const ds_math::Vector3 &velocity);

        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setVelocity(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getVelocity(ds_math::Vector3 *velocity) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getVelocity() const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addVelocity(const ds_math::Vector3 &deltaVelocity);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setRotation(const ds_math::Vector3 &rotation);

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setRotation(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getRotation(ds_math::Vector3 *rotation) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getRotation() const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addRotation(const ds_math::Vector3 &deltaRotation);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        bool getAwake() const
        {
            return isAwake;
        }

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setAwake(const bool awake=true);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        bool getCanSleep() const
        {
            return canSleep;
        }

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setCanSleep(const bool canSleep=true);

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
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getLastFrameAcceleration(ds_math::Vector3 *linearAcceleration) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getLastFrameAcceleration() const;

        /*@}*/


        /**
         * @name Force, Torque and Acceleration Set-up Functions
         *
         * These functions set up forces and torques to apply to the
         * rigid body.
         */
        /*@{*/

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void clearAccumulators();

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addForce(const ds_math::Vector3 &force);

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addForceAtPoint(const ds_math::Vector3 &force, const ds_math::Vector3 &point);

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addForceAtBodyPoint(const ds_math::Vector3 &force, const ds_math::Vector3 &point);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void addTorque(const ds_math::Vector3 &torque);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setAcceleration(const ds_math::Vector3 &acceleration);

        ///
        ///
        ///
        ///
        ///
        ///
        /// ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void setAcceleration(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z);

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        void getAcceleration(ds_math::Vector3 *acceleration) const;

        ///
        ///
        ///
        ///
        ///
        ///
        /// 
        ds_math::Vector3 getAcceleration() const;

        /*@}*/
    
    

protected:
    
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
    ///
    ///
    ///
    ///
    ///
    ds_math::Vector3 m_position;


    ///
    /// Holds inverse mass of rigid body
    /// Inverse is one over mass, makes intergration simpler
    /// More useful to have infinite mass than zero mass
    /// 
    ///
    ds_math::scalar inverseMass;
    
    ///
    /// Holds inverse of rigid body inertia tensor
    /// Must not degenerate (have zero inertia along one axis)
    /// Used for similar reasons than inverse mass
    /// Is given in body space (unlike other rigid body variables)
    ///
    ds_math::Matrix3 inverseInertiaTensor;
    
    ///
    /// Holds amount of damping applied to linear motion
    /// Required to remove energy added through numerical instability to the integrator
    ///
    ///
    ///
    ds_math::scalar linearDamping;
    
    ///
    /// Holds amount of damping applied to angular motion
    /// Required to remove energy added through numerical instability to the integrator
    ///
    ///
    ///
    ds_math::scalar angularDamping;
    
    ///
    /// Holds the linear position of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 position;
    
    ///
    /// Holds the angular orientation of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Quaternion orientation;

    ///
    /// Holds the linear velocity of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 velocity;
    
    ///
    /// Holds the angular velocity (rotation) of the rigid body in world space
    ///
    ///
    ///
    ///
    ds_math::Vector3 rotation;

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
    ds_math::Matrix3 inverseInertiaTensorWorld;

    ///
    /// Holds the amount  of motion of the body
    /// Recency weighted mean that can be used to put the body to sleep
    /// (Weighted towards what has recently been used)
    ///
    ///
    ds_math::scalar motion;

    ///
    /// Can be put to sleep to avoid being updated by integration functions
    /// or effected by collisions with the world
    ///
    ///
    ///
    bool isAwake;

    ///
    /// Some rigid bodies may never be asleep
    /// @Example - User controlled bodies should always be awake
    ///
    ///
    ///
    bool canSleep;

    ///
    /// Holds transform matrix for converting body space into world space (and vice versa)
    /// Acheived by calling the GetPointIn*Space functions
    ///
    /// @see getPointInLocalSpace
    /// @see getPointInWorldSpace
    /// @see getTransform
    ///
    ds_Math::ds_math::Matrix4 transformMatrix;

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
    ds_math::Vector3 forceAccum;

    ///
    /// Holds the accumulated torque to be applied at the next integration step
    ///
    ///
    ///
    ///
    ds_math::Vector3 torqueAccum;

    ///
    /// Holds acceleration of the rigid body
    /// This value can be used to set acceleration due to gravity (primary use)
    /// or other acceleration
    ///
    ///
    ds_math::Vector3 acceleration;

    ///
    /// Holds the linear acceleration of the rigid body for previous frame
    ///
    ///
    ///
    ///
    ds_math::Vector3 lastFrameAcceleration;

    /*@}*/


private:
};
}
