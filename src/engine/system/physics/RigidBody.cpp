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
#include <cassert>

#include "engine/system/physics/PhysicsCore.h"
#include "engine/system/physics/RigidBody.h"

namespace ds_phys
{

///
/// Internal function that checks the validity of an inverse inertia tensor.
///
static inline void checkInverseInertiaTensor(const ds_math::Matrix3 &iitWorld)
{
}

///
/// Internal function to do an inertia tensor transformation by a Quaternion.
///
static inline void transformInertiaTensor(ds_math::Matrix3 &iitWorld,
                                          const ds_math::Quaternion &q,
                                          const ds_math::Matrix3 &iitBody,
                                          const ds_math::Matrix4 &rotmat)
{
    ds_math::scalar t4 = rotmat.data[0][0] * iitBody.data[0][0] +
                         rotmat.data[1][0] * iitBody.data[0][1] +
                         rotmat.data[2][0] * iitBody.data[0][2];
    ds_math::scalar t9 = rotmat.data[0][0] * iitBody.data[1][0] +
                         rotmat.data[1][0] * iitBody.data[1][1] +
                         rotmat.data[2][0] * iitBody.data[1][2];
    ds_math::scalar t14 = rotmat.data[0][0] * iitBody.data[2][0] +
                          rotmat.data[1][0] * iitBody.data[2][1] +
                          rotmat.data[2][0] * iitBody.data[2][2];
    ds_math::scalar t28 = rotmat.data[0][1] * iitBody.data[0][0] +
                          rotmat.data[1][1] * iitBody.data[0][1] +
                          rotmat.data[2][1] * iitBody.data[0][2];
    ds_math::scalar t33 = rotmat.data[0][1] * iitBody.data[1][0] +
                          rotmat.data[1][1] * iitBody.data[1][1] +
                          rotmat.data[2][1] * iitBody.data[1][2];
    ds_math::scalar t38 = rotmat.data[0][1] * iitBody.data[2][0] +
                          rotmat.data[1][1] * iitBody.data[2][1] +
                          rotmat.data[2][1] * iitBody.data[2][2];
    ds_math::scalar t52 = rotmat.data[0][2] * iitBody.data[0][0] +
                          rotmat.data[1][2] * iitBody.data[0][1] +
                          rotmat.data[2][2] * iitBody.data[0][2];
    ds_math::scalar t57 = rotmat.data[0][2] * iitBody.data[1][0] +
                          rotmat.data[1][2] * iitBody.data[1][1] +
                          rotmat.data[2][2] * iitBody.data[1][2];
    ds_math::scalar t62 = rotmat.data[0][2] * iitBody.data[2][0] +
                          rotmat.data[1][2] * iitBody.data[2][1] +
                          rotmat.data[2][2] * iitBody.data[2][2];

    iitWorld.data[0][0] = t4 * rotmat.data[0][0] + t9 * rotmat.data[1][0] +
                          t14 * rotmat.data[2][0];
    iitWorld.data[0][1] = t4 * rotmat.data[0][1] + t9 * rotmat.data[1][1] +
                          t14 * rotmat.data[2][1];
    iitWorld.data[0][2] = t4 * rotmat.data[0][2] + t9 * rotmat.data[1][2] +
                          t14 * rotmat.data[2][2];
    iitWorld.data[1][0] = t28 * rotmat.data[0][0] + t33 * rotmat.data[1][0] +
                          t38 * rotmat.data[2][0];
    iitWorld.data[1][1] = t28 * rotmat.data[0][1] + t33 * rotmat.data[1][1] +
                          t38 * rotmat.data[2][1];
    iitWorld.data[1][2] = t28 * rotmat.data[0][2] + t33 * rotmat.data[1][2] +
                          t38 * rotmat.data[2][2];
    iitWorld.data[2][0] = t52 * rotmat.data[0][0] + t57 * rotmat.data[1][0] +
                          t62 * rotmat.data[2][0];
    iitWorld.data[2][1] = t52 * rotmat.data[0][1] + t57 * rotmat.data[1][1] +
                          t62 * rotmat.data[2][1];
    iitWorld.data[2][2] = t52 * rotmat.data[0][2] + t57 * rotmat.data[1][2] +
                          t62 * rotmat.data[2][2];
}

///
/// Internal function that creates a tranform matrix from a position and
/// rotation.
///
static inline void
calculateTransformMatrix(ds_math::Matrix4 &transformMatrix,
                         const ds_math::Vector3 &position,
                         const ds_math::Quaternion &orientation)
{
    transformMatrix.data[0][0] = 1 - 2 * orientation.y * orientation.y -
                                 2 * orientation.z * orientation.z;
    transformMatrix.data[1][0] =
        2 * orientation.x * orientation.y - 2 * orientation.w * orientation.z;
    transformMatrix.data[2][0] =
        2 * orientation.x * orientation.z + 2 * orientation.w * orientation.y;
    transformMatrix.data[3][0] = position.x;

    transformMatrix.data[0][1] =
        2 * orientation.x * orientation.y + 2 * orientation.w * orientation.z;
    transformMatrix.data[1][1] = 1 - 2 * orientation.x * orientation.x -
                                 2 * orientation.z * orientation.z;
    transformMatrix.data[2][1] =
        2 * orientation.y * orientation.z - 2 * orientation.w * orientation.x;
    transformMatrix.data[3][1] = position.y;

    transformMatrix.data[0][2] =
        2 * orientation.x * orientation.z - 2 * orientation.w * orientation.y;
    transformMatrix.data[1][2] =
        2 * orientation.y * orientation.z + 2 * orientation.w * orientation.x;
    transformMatrix.data[2][2] = 1 - 2 * orientation.x * orientation.x -
                                 2 * orientation.y * orientation.y;
    transformMatrix.data[3][2] = position.z;
}

/// Functions declared in header.

RigidBody::RigidBody()
    : m_inverseMass((ds_math::scalar)0.0),
      m_linearDamping((ds_math::scalar)0.95),
      m_angularDamping((ds_math::scalar)0.8),
      m_motion((ds_math::scalar)0.0),
      m_isAwake(true),
      m_canSleep(true)
{
}

void RigidBody::integrate(ds_math::scalar duration)
{
    if (!m_isAwake)
    {
        return;
    }

    // Calculate linear acceleration from force inputs.
    m_lastFrameAcceleration = m_acceleration;
    m_lastFrameAcceleration += m_forceAccum * m_inverseMass;

    // Calculate angular acceleration from torque inputs.
    ds_math::Vector3 angularAcceleration =
        ds_math::Matrix3::Transform(m_inverseInertiaTensorWorld, m_torqueAccum);

    /// Update linear velocity from acceleration and impulse.
    m_velocity += m_lastFrameAcceleration * duration;

    // Update angular velocity from acceleration and impulse.
    m_rotation += angularAcceleration * duration;

    // Impose drag.
    m_velocity *= SCALAR_POW(m_linearDamping, duration);
    m_rotation *= SCALAR_POW(m_angularDamping, duration);

    // Update linear position.
    m_position += m_velocity * duration;

    // Update angular position.
    m_orientation.AddScaledVector(m_rotation, duration);

    // Normalize the orientation, and update the matrices with the new
    // position and orientation
    calculateDerivedData();

    // Clear accumulators.
    clearAccumulators();

    // Update kinetic energy store, and possibly put the body to sleep.
    if (m_canSleep)
    {
        ds_math::scalar currentMotion =
            ds_math::Vector3::Dot(m_velocity, m_velocity) +
            ds_math::Vector3::Dot(m_rotation, m_rotation);

        ds_math::scalar bias = SCALAR_POW(0.5, duration);
        m_motion = bias * m_motion + (1 - bias) * currentMotion;

        if (m_motion < sleepEpsilon)
        {
            setAwake(false);
        }
        else if (m_motion > 10 * sleepEpsilon)
        {
            m_motion = 10 * sleepEpsilon;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////JULES
/// STUFF//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RigidBody::calculateDerivedData()
{
    m_orientation.Normalize();

    // Calculate the transform matrix for the body.
    calculateTransformMatrix(m_transformMatrix, m_position, m_orientation);

    // Calculate the inertiaTensor in world space.
    transformInertiaTensor(m_inverseInertiaTensorWorld, m_orientation,
                           m_inverseInertiaTensor, m_transformMatrix);
}

void RigidBody::setMass(const ds_math::scalar mass)
{
    assert(mass != 0);

    m_inverseMass = ((ds_math::scalar)1.0) / mass;
}

ds_math::scalar RigidBody::getMass() const
{
    if (m_inverseMass == 0)
    {
        return ds_math::SCALAR_MAX;
    }
    else
    {
        return ((ds_math::scalar)1.0) / m_inverseMass;
    }
}

void RigidBody::setInverseMass(const ds_math::scalar inverseMass)
{
    m_inverseMass = inverseMass;
}

ds_math::scalar RigidBody::getInverseMass() const
{
    return m_inverseMass;
}

bool RigidBody::hasFiniteMass() const
{
    return m_inverseMass > 0.0f;
}

void RigidBody::setInertiaTensor(const ds_math::Matrix3 &inertiaTensor)
{
    m_inverseInertiaTensor = ds_math::Matrix3::Inverse(inertiaTensor);
    checkInverseInertiaTensor(m_inverseInertiaTensor);
}

void RigidBody::getInertiaTensor(ds_math::Matrix3 *inertiaTensor) const
{
    assert(inertiaTensor != nullptr);

    *inertiaTensor = ds_math::Matrix3::Inverse(m_inverseInertiaTensor);
}

ds_math::Matrix3 RigidBody::getInertiaTensor() const
{
    ds_math::Matrix3 it;

    getInertiaTensor(&it);

    return it;
}

void RigidBody::getInertiaTensorWorld(ds_math::Matrix3 *inertiaTensor) const
{
    assert(inertiaTensor != nullptr);

    *inertiaTensor = ds_math::Matrix3::Inverse(m_inverseInertiaTensorWorld);
}

ds_math::Matrix3 RigidBody::getInertiaTensorWorld() const
{
    ds_math::Matrix3 it;
    getInertiaTensorWorld(&it);
    return it;
}

void RigidBody::setInverseInertiaTensor(
    const ds_math::Matrix3 &inverseInertiaTensor)
{
    checkInverseInertiaTensor(inverseInertiaTensor);
    m_inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensor(
    ds_math::Matrix3 *inverseInertiaTensor) const
{
    assert(inverseInertiaTensor != nullptr);

    *inverseInertiaTensor = m_inverseInertiaTensor;
}

ds_math::Matrix3 RigidBody::getInverseInertiaTensor() const
{
    return m_inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensorWorld(
    ds_math::Matrix3 *inverseInertiaTensor) const
{
    assert(inverseInertiaTensor != nullptr);

    *inverseInertiaTensor = m_inverseInertiaTensorWorld;
}

ds_math::Matrix3 RigidBody::getInverseInertiaTensorWorld() const
{
    return m_inverseInertiaTensorWorld;
}

void RigidBody::setDamping(const ds_math::scalar linearDamping,
                           const ds_math::scalar angularDamping)
{
    m_linearDamping = linearDamping;
    m_angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const ds_math::scalar linearDamping)
{
    m_linearDamping = linearDamping;
}

ds_math::scalar RigidBody::getLinearDamping() const
{
    return m_linearDamping;
}

void RigidBody::setAngularDamping(const ds_math::scalar angularDamping)
{
    m_angularDamping = angularDamping;
}

ds_math::scalar RigidBody::getAngularDamping() const
{
    return m_angularDamping;
}

void RigidBody::setPosition(const ds_math::Vector3 &position)
{
    m_position = position;
}

void RigidBody::setPosition(const ds_math::scalar x,
                            const ds_math::scalar y,
                            const ds_math::scalar z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void RigidBody::getPosition(ds_math::Vector3 *position) const
{
    assert(position != nullptr);

    *position = m_position;
}

ds_math::Vector3 RigidBody::getPosition() const
{
    return m_position;
}

void RigidBody::setOrientation(const ds_math::Quaternion &orientation)
{
    m_orientation = orientation;
    m_orientation.Normalize();
}

void RigidBody::setOrientation(const ds_math::scalar r,
                               const ds_math::scalar i,
                               const ds_math::scalar j,
                               const ds_math::scalar k)
{
    m_orientation.w = r;
    m_orientation.x = i;
    m_orientation.y = j;
    m_orientation.z = k;
    m_orientation.Normalize();
}

void RigidBody::getOrientation(ds_math::Quaternion *orientation) const
{
    assert(orientation != nullptr);

    *orientation = m_orientation;
}

ds_math::Quaternion RigidBody::getOrientation() const
{
    return m_orientation;
}

void RigidBody::getOrientation(ds_math::Matrix3 *matrix) const
{
    assert(matrix != nullptr);

    getOrientation(&(matrix->data[0][0]));
}

// TODO: Remove matrix[9]?
void RigidBody::getOrientation(ds_math::scalar matrix[9]) const
{
    matrix[0] = m_transformMatrix.data[0][0];
    matrix[1] = m_transformMatrix.data[0][1];
    matrix[2] = m_transformMatrix.data[0][2];

    matrix[3] = m_transformMatrix.data[1][0];
    matrix[4] = m_transformMatrix.data[1][1];
    matrix[5] = m_transformMatrix.data[1][2];

    matrix[6] = m_transformMatrix.data[2][0];
    matrix[7] = m_transformMatrix.data[2][1];
    matrix[8] = m_transformMatrix.data[2][2];
}

void RigidBody::getTransform(ds_math::Matrix4 *transform) const
{
    assert(transform != nullptr);

    memcpy(transform, &m_transformMatrix.data, sizeof(ds_math::Matrix4));
}

void RigidBody::getTransform(ds_math::scalar matrix[16]) const
{
    memcpy(matrix, m_transformMatrix.data, sizeof(ds_math::scalar) * 12);
    matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void RigidBody::getGLTransform(float matrix[16]) const
{
    matrix[0] = (float)m_transformMatrix.data[0][0];
    matrix[1] = (float)m_transformMatrix.data[1][0];
    matrix[2] = (float)m_transformMatrix.data[2][0];
    matrix[3] = 0;

    matrix[4] = (float)m_transformMatrix.data[0][1];
    matrix[5] = (float)m_transformMatrix.data[1][1];
    matrix[6] = (float)m_transformMatrix.data[2][1];
    matrix[7] = 0;

    matrix[8] = (float)m_transformMatrix.data[0][2];
    matrix[9] = (float)m_transformMatrix.data[1][2];
    matrix[10] = (float)m_transformMatrix.data[2][2];
    matrix[11] = 0;

    matrix[12] = (float)m_transformMatrix.data[0][3];
    matrix[13] = (float)m_transformMatrix.data[1][3];
    matrix[14] = (float)m_transformMatrix.data[2][3];
    matrix[15] = 1;
}

ds_math::Matrix4 RigidBody::getTransform() const
{
    return m_transformMatrix;
}

ds_math::Vector3
RigidBody::getPointInLocalSpace(const ds_math::Vector3 &point) const
{
    return ds_math::Matrix4::TransformInverse(m_transformMatrix, point);
}

ds_math::Vector3
RigidBody::getPointInWorldSpace(const ds_math::Vector3 &point) const
{
    return ds_math::Matrix4::Transform(m_transformMatrix, point);
}

ds_math::Vector3
RigidBody::getDirectionInLocalSpace(const ds_math::Vector3 &direction) const
{
    return ds_math::Matrix4::TransformInverseDirection(m_transformMatrix,
                                                       direction);
}

ds_math::Vector3
RigidBody::getDirectionInWorldSpace(const ds_math::Vector3 &direction) const
{
    return ds_math::Matrix4::TransformDirection(m_transformMatrix, direction);
}

void RigidBody::setVelocity(const ds_math::Vector3 &velocity)
{
    m_velocity = velocity;
}

void RigidBody::setVelocity(const ds_math::scalar x,
                            const ds_math::scalar y,
                            const ds_math::scalar z)
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

void RigidBody::getVelocity(ds_math::Vector3 *velocity) const
{
    assert(velocity != nullptr);

    *velocity = m_velocity;
}

ds_math::Vector3 RigidBody::getVelocity() const
{
    return m_velocity;
}

void RigidBody::addVelocity(const ds_math::Vector3 &deltaVelocity)
{
    m_velocity += deltaVelocity;
}

void RigidBody::setRotation(const ds_math::Vector3 &rotation)
{
    m_rotation = rotation;
}

void RigidBody::setRotation(const ds_math::scalar x,
                            const ds_math::scalar y,
                            const ds_math::scalar z)
{
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
}

void RigidBody::getRotation(ds_math::Vector3 *rotation) const
{
    assert(rotation != nullptr);

    *rotation = m_rotation;
}

ds_math::Vector3 RigidBody::getRotation() const
{
    return m_rotation;
}

void RigidBody::addRotation(const ds_math::Vector3 &deltaRotation)
{
    m_rotation += deltaRotation;
}

void RigidBody::setAwake(const bool awake)
{
    if (awake)
    {
        m_isAwake = true;

        // Add a bit of motion to avoid it falling asleep immediately.
        m_motion = sleepEpsilon * 2.0f;
    }
    else
    {
        m_isAwake = false;
        m_velocity.Clear();
        m_rotation.Clear();
    }
}

void RigidBody::setCanSleep(const bool canSleep)
{
    m_canSleep = canSleep;

    if (!m_canSleep && !m_isAwake)
        setAwake();
}


void RigidBody::getLastFrameAcceleration(ds_math::Vector3 *acceleration) const
{
    assert(acceleration != nullptr);

    *acceleration = m_lastFrameAcceleration;
}

ds_math::Vector3 RigidBody::getLastFrameAcceleration() const
{
    return m_lastFrameAcceleration;
}

void RigidBody::clearAccumulators()
{
    m_forceAccum.Clear();
    m_torqueAccum.Clear();
}

void RigidBody::addForce(const ds_math::Vector3 &force)
{
    m_forceAccum += force;
    m_isAwake = true;
}

void RigidBody::addForceAtBodyPoint(const ds_math::Vector3 &force,
                                    const ds_math::Vector3 &point)
{
    // Convert to coordinates relative to center of mass.
    ds_math::Vector3 pt = getPointInWorldSpace(point);
    addForceAtPoint(force, pt);
}

void RigidBody::addForceAtPoint(const ds_math::Vector3 &force,
                                const ds_math::Vector3 &point)
{
    // Convert to coordinates relative to center of mass.
    ds_math::Vector3 pt = point;
    pt -= m_position;

    m_forceAccum += force;
    m_torqueAccum += ds_math::Vector3::Cross(pt, force);

    m_isAwake = true;
}

void RigidBody::addTorque(const ds_math::Vector3 &torque)
{
    m_torqueAccum += torque;
    m_isAwake = true;
}

void RigidBody::setAcceleration(const ds_math::Vector3 &acceleration)
{
    m_acceleration = acceleration;
}

void RigidBody::setAcceleration(const ds_math::scalar x,
                                const ds_math::scalar y,
                                const ds_math::scalar z)
{
    m_acceleration.x = x;
    m_acceleration.y = y;
    m_acceleration.z = z;
}

void RigidBody::getAcceleration(ds_math::Vector3 *acceleration) const
{
    *acceleration = m_acceleration;
}

ds_math::Vector3 RigidBody::getAcceleration() const
{
    return m_acceleration;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////JULES
/// STUFF////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
