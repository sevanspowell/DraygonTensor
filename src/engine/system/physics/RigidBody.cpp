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
}
