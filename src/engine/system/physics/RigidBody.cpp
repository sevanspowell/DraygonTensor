#include "engine/system/physics/RigidBody.h"

namespace ds_phys
{

///
/// Internal function that checks the validity of an inverse inertia tensor.
///
	static inline void _checkInverseInertiaTensor(const ds_math::Matrix3 &iitWorld)
	{

	}

	///
	/// Internal function to do an inertia tensor transformation by a Quaternion.
	///
	static inline void _transformInertiaTensor(ds_math::Matrix3 &iitWorld, const ds_math::Quaternion &q, const ds_math::Matrix3 &iitBody, const ds_math::Matrix4 &rotmat)
	{
		ds_math::scalar t4 = rotmat.data[0] * iitBody.data[0] +
			rotmat.data[1] * iitBody.data[3] +
			rotmat.data[2] * iitBody.data[6];
		ds_math::scalar t9 = rotmat.data[0] * iitBody.data[1] +
			rotmat.data[1] * iitBody.data[4] +
			rotmat.data[2] * iitBody.data[7];
		ds_math::scalar t14 = rotmat.data[0] * iitBody.data[2] +
			rotmat.data[1] * iitBody.data[5] +
			rotmat.data[2] * iitBody.data[8];
		ds_math::scalar t28 = rotmat.data[4] * iitBody.data[0] +
			rotmat.data[5] * iitBody.data[3] +
			rotmat.data[6] * iitBody.data[6];
		ds_math::scalar t33 = rotmat.data[4] * iitBody.data[1] +
			rotmat.data[5] * iitBody.data[4] +
			rotmat.data[6] * iitBody.data[7];
		ds_math::scalar t38 = rotmat.data[4] * iitBody.data[2] +
			rotmat.data[5] * iitBody.data[5] +
			rotmat.data[6] * iitBody.data[8];
		ds_math::scalar t52 = rotmat.data[8] * iitBody.data[0] +
			rotmat.data[9] * iitBody.data[3] +
			rotmat.data[10] * iitBody.data[6];
		ds_math::scalar t57 = rotmat.data[8] * iitBody.data[1] +
			rotmat.data[9] * iitBody.data[4] +
			rotmat.data[10] * iitBody.data[7];
		ds_math::scalar t62 = rotmat.data[8] * iitBody.data[2] +
			rotmat.data[9] * iitBody.data[5] +
			rotmat.data[10] * iitBody.data[8];

		iitWorld.data[0] = t4*rotmat.data[0] +
			t9*rotmat.data[1] +
			t14*rotmat.data[2];
		iitWorld.data[1] = t4*rotmat.data[4] +
			t9*rotmat.data[5] +
			t14*rotmat.data[6];
		iitWorld.data[2] = t4*rotmat.data[8] +
			t9*rotmat.data[9] +
			t14*rotmat.data[10];
		iitWorld.data[3] = t28*rotmat.data[0] +
			t33*rotmat.data[1] +
			t38*rotmat.data[2];
		iitWorld.data[4] = t28*rotmat.data[4] +
			t33*rotmat.data[5] +
			t38*rotmat.data[6];
		iitWorld.data[5] = t28*rotmat.data[8] +
			t33*rotmat.data[9] +
			t38*rotmat.data[10];
		iitWorld.data[6] = t52*rotmat.data[0] +
			t57*rotmat.data[1] +
			t62*rotmat.data[2];
		iitWorld.data[7] = t52*rotmat.data[4] +
			t57*rotmat.data[5] +
			t62*rotmat.data[6];
		iitWorld.data[8] = t52*rotmat.data[8] +
			t57*rotmat.data[9] +
			t62*rotmat.data[10];
	}

	///
	/// Internal function that creates a tranform matrix from a position and rotation.
	///
	static inline void _calculateTransformMatrix(ds_math::Matrix4 &transformMatrix, const ds_math::Vector3 &position, const ds_math::Quaternion &orientation)
	{
		transformMatrix.data[0] = 1 - 2 * orientation.j*orientation.j -
			2 * orientation.k*orientation.k;
		transformMatrix.data[1] = 2 * orientation.i*orientation.j -
			2 * orientation.r*orientation.k;
		transformMatrix.data[2] = 2 * orientation.i*orientation.k +
			2 * orientation.r*orientation.j;
		transformMatrix.data[3] = position.x;

		transformMatrix.data[4] = 2 * orientation.i*orientation.j +
			2 * orientation.r*orientation.k;
		transformMatrix.data[5] = 1 - 2 * orientation.i*orientation.i -
			2 * orientation.k*orientation.k;
		transformMatrix.data[6] = 2 * orientation.j*orientation.k -
			2 * orientation.r*orientation.i;
		transformMatrix.data[7] = position.y;

		transformMatrix.data[8] = 2 * orientation.i*orientation.k -
			2 * orientation.r*orientation.j;
		transformMatrix.data[9] = 2 * orientation.j*orientation.k +
			2 * orientation.r*orientation.i;
		transformMatrix.data[10] = 1 - 2 * orientation.i*orientation.i -
			2 * orientation.j*orientation.j;
		transformMatrix.data[11] = position.z;
	}

	///Functions declared in header.

	void RigidBody::calculateDerivedData()
	{
		orientation.normalise();

		//Calculate the transform matrix for the body.
		_calculateTransformMatrix(m_transformMatrix, m_position, m_orientation);

		//Calculate the inertiaTensor in world space.
		_transformInertiaTensor(m_inverseInertiaTensorWorld, m_orientation, m_inverseInertiaTensor, m_transformMatrix);
	}

void RigidBody::integrate(ds_math::scalar duration)
{
	if (!isAwake)
	{
		return;
	}

	//Calculate linear acceleration from force inputs.
	m_lastFrameAcceleration = m_acceleration;
	m_lastFrameAcceleration.addScaledVector(m_forceAccum, m_inverseMass);

	//Calculate angular acceleration from torque inputs.
	ds_math::Vector3 angularAcceleration = m_inverseInertiaTensorWorld.transform(m_torqueAccum);

	///Update linear velocity from acceleration and impulse.
	m_velocity.addScaledVector(m_lastFrameAcceleration, duration);

	//Update angular velocity from acceleration and impulse.
	m_rotation.addScaledVector(m_angularAcceleration, duration);

	//Impose drag.
	m_velocity *= ds_math::scalarPow(m_linearDamping, duration);
	m_rotation *= ds_math::scalarPow(m_angularDamping, duration);

	//Update linear position.
	m_position.addScaledVector(m_velocity, duration);

	//Update angular position.
	m_orientation.addScaledVector(m_rotation, duration);

	//Normalise the orientation and update the matrices.
	calculateAccumulators();

	//Update kinetic energy store, and possibly put the body to sleep.
	if (canSleep)
	{
		ds_math::scalar currentMotion = ds_math::Vector3::Dot(m_velocity, m_velocity) + ds_math::Vector3::Dot(m_rotation, m_rotation);

		ds_math::scalar bias = ds_math::scalarPow(0.5, duration);
		m_motion = bias*m_motion + (1 - bias)*currentMotion;

		if (m_motion < m_sleepEpsilon)
		{
			setAwake(false);
		}
		else if (m_motion > 10 * m_sleepEpsilon)
		{
			m_motion = 10 * m_sleepEpsilon;
		}
	}

}

//////// BOOKMARK - Luke Phipps 5:10pm 31/08/2016 /////////

////////// @jules - commented this out because they exist below, leaving them here until next group coding session to be sure
/*
void RigidBody::clearAccumulators()
{

}

void RigidBody::calculateDerivedData()
{

}

ds_math::Vector3 RigidBody::getPosition() const
{
    return m_position;
}

void RigidBody::setPosition(const ds_math::Vector3 &position)
{
    m_position = position;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////JULES STUFF//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RigidBody::calculateDerivedData()
{
    orientation.normalise();

    // Calculate the transform matrix for the body.
    _calculateTransformMatrix(transformMatrix, position, orientation);

    // Calculate the inertiaTensor in world space.
    _transformInertiaTensor(inverseInertiaTensorWorld,
        orientation,
        inverseInertiaTensor,
        transformMatrix);

}

void RigidBody::integrate(ds_math::scalar duration)
{
    if (!isAwake) return;

    // Calculate linear acceleration from force inputs.
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

    // Calculate angular acceleration from torque inputs.
    ds_math::Vector3 angularAcceleration =
        inverseInertiaTensorWorld.transform(torqueAccum);

    // Adjust velocities
    // Update linear velocity from both acceleration and impulse.
    velocity.addScaledVector(lastFrameAcceleration, duration);

    // Update angular velocity from both acceleration and impulse.
    rotation.addScaledVector(angularAcceleration, duration);

    // Impose drag.
    velocity *= real_pow(linearDamping, duration);
    rotation *= real_pow(angularDamping, duration);

    // Adjust positions
    // Update linear position.
    position.addScaledVector(velocity, duration);

    // Update angular position.
    orientation.addScaledVector(rotation, duration);

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    calculateDerivedData();

    // Clear accumulators.
    clearAccumulators();

    // Update the kinetic energy store, and possibly put the body to
    // sleep.
    if (canSleep) {
        ds_math::scalar currentMotion = velocity.scalarProduct(velocity) +
            rotation.scalarProduct(rotation);

        ds_math::scalar bias = real_pow(0.5, duration);
        motion = bias*motion + (1-bias)*currentMotion;

        if (motion < sleepEpsilon) setAwake(false);
        else if (motion > 10 * sleepEpsilon) motion = 10 * sleepEpsilon;
    }
}

void RigidBody::setMass(const ds_math::scalar mass)
{
    assert(mass != 0);
    RigidBody::inverseMass = ((ds_math::scalar)1.0)/mass;
}

ds_math::scalar RigidBody::getMass() const
{
    if (inverseMass == 0) {
        return REAL_MAX;
    } else {
        return ((ds_math::scalar)1.0)/inverseMass;
    }
}

void RigidBody::setInverseMass(const ds_math::scalar inverseMass)
{
    RigidBody::inverseMass = inverseMass;
}

ds_math::scalar RigidBody::getInverseMass() const
{
    return inverseMass;
}

bool RigidBody::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void RigidBody::setInertiaTensor(const ds_math::Matrix3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
    _checkInverseInertiaTensor(inverseInertiaTensor);
}

void RigidBody::getInertiaTensor(ds_math::Matrix3 *inertiaTensor) const
{
    inertiaTensor->setInverse(inverseInertiaTensor);
}

ds_math::Matrix3 RigidBody::getInertiaTensor() const
{
    ds_math::Matrix3 it;
    getInertiaTensor(&it);
    return it;
}

void RigidBody::getInertiaTensorWorld(ds_math::Matrix3 *inertiaTensor) const
{
    inertiaTensor->setInverse(inverseInertiaTensorWorld);
}

ds_math::Matrix3 RigidBody::getInertiaTensorWorld() const
{
    ds_math::Matrix3 it;
    getInertiaTensorWorld(&it);
    return it;
}

void RigidBody::setInverseInertiaTensor(const ds_math::Matrix3 &inverseInertiaTensor)
{
    _checkInverseInertiaTensor(inverseInertiaTensor);
    RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensor(ds_math::Matrix3 *inverseInertiaTensor) const
{
    *inverseInertiaTensor = RigidBody::inverseInertiaTensor;
}

ds_math::Matrix3 RigidBody::getInverseInertiaTensor() const
{
    return inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensorWorld(ds_math::Matrix3 *inverseInertiaTensor) const
{
    *inverseInertiaTensor = inverseInertiaTensorWorld;
}

ds_math::Matrix3 RigidBody::getInverseInertiaTensorWorld() const
{
    return inverseInertiaTensorWorld;
}

void RigidBody::setDamping(const ds_math::scalar linearDamping,
               const ds_math::scalar angularDamping)
{
    RigidBody::linearDamping = linearDamping;
    RigidBody::angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const ds_math::scalar linearDamping)
{
    RigidBody::linearDamping = linearDamping;
}

ds_math::scalar RigidBody::getLinearDamping() const
{
    return linearDamping;
}

void RigidBody::setAngularDamping(const ds_math::scalar angularDamping)
{
    RigidBody::angularDamping = angularDamping;
}

ds_math::scalar RigidBody::getAngularDamping() const
{
    return angularDamping;
}

void RigidBody::setPosition(const ds_math::Vector3 &position)
{
    RigidBody::position = position;
}

void RigidBody::setPosition(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void RigidBody::getPosition(ds_math::Vector3 *position) const
{
    *position = RigidBody::position;
}

ds_math::Vector3 RigidBody::getPosition() const
{
    return position;
}

void RigidBody::setOrientation(const Quaternion &orientation)
{
    RigidBody::orientation = orientation;
    RigidBody::orientation.normalise();
}

void RigidBody::setOrientation(const ds_math::scalar r, const ds_math::scalar i,
                   const ds_math::scalar j, const ds_math::scalar k)
{
    orientation.r = r;
    orientation.i = i;
    orientation.j = j;
    orientation.k = k;
    orientation.normalise();
}

void RigidBody::getOrientation(Quaternion *orientation) const
{
    *orientation = RigidBody::orientation;
}

Quaternion RigidBody::getOrientation() const
{
    return orientation;
}

void RigidBody::getOrientation(ds_math::Matrix3 *matrix) const
{
    getOrientation(matrix->data);
}

void RigidBody::getOrientation(ds_math::scalar matrix[9]) const
{
    matrix[0] = transformMatrix.data[0];
    matrix[1] = transformMatrix.data[1];
    matrix[2] = transformMatrix.data[2];

    matrix[3] = transformMatrix.data[4];
    matrix[4] = transformMatrix.data[5];
    matrix[5] = transformMatrix.data[6];

    matrix[6] = transformMatrix.data[8];
    matrix[7] = transformMatrix.data[9];
    matrix[8] = transformMatrix.data[10];
}

void RigidBody::getTransform(ds_math::Matrix4 *transform) const
{
    memcpy(transform, &transformMatrix.data, sizeof(ds_math::Matrix4));
}

void RigidBody::getTransform(ds_math::scalar matrix[16]) const
{
    memcpy(matrix, transformMatrix.data, sizeof(ds_math::scalar)*12);
    matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void RigidBody::getGLTransform(float matrix[16]) const
{
    matrix[0] = (float)transformMatrix.data[0];
    matrix[1] = (float)transformMatrix.data[4];
    matrix[2] = (float)transformMatrix.data[8];
    matrix[3] = 0;

    matrix[4] = (float)transformMatrix.data[1];
    matrix[5] = (float)transformMatrix.data[5];
    matrix[6] = (float)transformMatrix.data[9];
    matrix[7] = 0;

    matrix[8] = (float)transformMatrix.data[2];
    matrix[9] = (float)transformMatrix.data[6];
    matrix[10] = (float)transformMatrix.data[10];
    matrix[11] = 0;

    matrix[12] = (float)transformMatrix.data[3];
    matrix[13] = (float)transformMatrix.data[7];
    matrix[14] = (float)transformMatrix.data[11];
    matrix[15] = 1;
}

ds_math::Matrix4 RigidBody::getTransform() const
{
    return transformMatrix;
}


ds_math::Vector3 RigidBody::getPointInLocalSpace(const ds_math::Vector3 &point) const
{
    return transformMatrix.transformInverse(point);
}

ds_math::Vector3 RigidBody::getPointInWorldSpace(const ds_math::Vector3 &point) const
{
    return transformMatrix.transform(point);
}

ds_math::Vector3 RigidBody::getDirectionInLocalSpace(const ds_math::Vector3 &direction) const
{
    return transformMatrix.transformInverseDirection(direction);
}

ds_math::Vector3 RigidBody::getDirectionInWorldSpace(const ds_math::Vector3 &direction) const
{
    return transformMatrix.transformDirection(direction);
}


void RigidBody::setVelocity(const ds_math::Vector3 &velocity)
{
    RigidBody::velocity = velocity;
}

void RigidBody::setVelocity(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void RigidBody::getVelocity(ds_math::Vector3 *velocity) const
{
    *velocity = RigidBody::velocity;
}

ds_math::Vector3 RigidBody::getVelocity() const
{
    return velocity;
}

void RigidBody::addVelocity(const ds_math::Vector3 &deltaVelocity)
{
    velocity += deltaVelocity;
}

void RigidBody::setRotation(const ds_math::Vector3 &rotation)
{
    RigidBody::rotation = rotation;
}

void RigidBody::setRotation(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void RigidBody::getRotation(ds_math::Vector3 *rotation) const
{
    *rotation = RigidBody::rotation;
}

ds_math::Vector3 RigidBody::getRotation() const
{
    return rotation;
}

void RigidBody::addRotation(const ds_math::Vector3 &deltaRotation)
{
    rotation += deltaRotation;
}

void RigidBody::setAwake(const bool awake)
{
    if (awake) {
        isAwake= true;

        // Add a bit of motion to avoid it falling asleep immediately.
        motion = sleepEpsilon*2.0f;
    } else {
        isAwake = false;
        velocity.clear();
        rotation.clear();
    }
}

void RigidBody::setCanSleep(const bool canSleep)
{
    RigidBody::canSleep = canSleep;

    if (!canSleep && !isAwake) setAwake();
}


void RigidBody::getLastFrameAcceleration(ds_math::Vector3 *acceleration) const
{
    *acceleration = lastFrameAcceleration;
}

ds_math::Vector3 RigidBody::getLastFrameAcceleration() const
{
    return lastFrameAcceleration;
}

void RigidBody::clearAccumulators()
{
    forceAccum.clear();
    torqueAccum.clear();
}

void RigidBody::addForce(const ds_math::Vector3 &force)
{
    forceAccum += force;
    isAwake = true;
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
    pt -= position;

    forceAccum += force;
    torqueAccum += pt % force;

    isAwake = true;
}

void RigidBody::addTorque(const ds_math::Vector3 &torque)
{
    torqueAccum += torque;
    isAwake = true;
}

void RigidBody::setAcceleration(const ds_math::Vector3 &acceleration)
{
    RigidBody::acceleration = acceleration;
}

void RigidBody::setAcceleration(const ds_math::scalar x, const ds_math::scalar y, const ds_math::scalar z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void RigidBody::getAcceleration(ds_math::Vector3 *acceleration) const
{
    *acceleration = RigidBody::acceleration;
}

ds_math::Vector3 RigidBody::getAcceleration() const
{
    return acceleration;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////JULES STUFF////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





}
