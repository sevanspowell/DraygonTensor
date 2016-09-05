#include <engine/system/physics/Contacts.h>
#include <algorithm>
#include <memory.h>
#include <assert.h>
#include <cstdlib>
#include <cstdio>

using namespace ds_phys;
using namespace ds_math;

void Contact::setBodyData(RigidBody* rb1, RigidBody* rb2, ds_math::scalar friction, ds_math::scalar restitution) {
	this->body[0] = rb1;
	this->body[1] = rb2;
	this->friction = friction;
	this->restitution = restitution;
}

void Contact::calculateInternals(ds_math::scalar duration) {
	///@todo Implement
}

void Contact::swapBodies() {
	contactNormal *= 1;
	std::swap(body[0], body[1]);
}

void Contact::matchAwakeState() {
	// World collisions never cause a body to wake up.
	if (!body[0] || !body[1]) return;

	bool isAwake0 = body[0]->getAwake();
	bool isAwake1 = body[1]->getAwake();
	if (isAwake0 ^ isAwake1) {
		if (isAwake0)
			body[1]->setAwake();
		else
			body[0]->setAwake();
	}
}

void Contact::calculateDesiredDeltaVelocity(ds_math::scalar duration) {
	Vector3 contactTangent[2];

	// Check which axis is closer to the z
	if (std::abs(contactNormal.x) > std::abs(contactNormal.y)) {

        // Scaling factor to ensure the results are normalised
        const scalar s = 1.0f/sqrt(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);

        // The new X-axis is at right angles to the world Y-axis
        contactTangent[0].x =  contactNormal.z*s;
        contactTangent[0].y =  0;
        contactTangent[0].z = -contactNormal.x*s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x =  contactNormal.y*contactTangent[0].x;
        contactTangent[1].y =  contactNormal.z*contactTangent[0].x - contactNormal.x*contactTangent[0].z;
        contactTangent[1].z = -contactNormal.y*contactTangent[0].x;
	} else {
        // Scaling factor to ensure the results are normalised
        const scalar s = 1.0/sqrt(contactNormal.z*contactNormal.z + contactNormal.y*contactNormal.y);

        // The new X-axis is at right angles to the world X-axis
        contactTangent[0].x = 0;
        contactTangent[0].y = -contactNormal.z*s;
        contactTangent[0].z =  contactNormal.y*s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x =  contactNormal.y*contactTangent[0].z - contactNormal.z*contactTangent[0].y;
        contactTangent[1].y = -contactNormal.x*contactTangent[0].z;
        contactTangent[1].z =  contactNormal.x*contactTangent[0].y;
	}
}

ds_math::Vector3 Contact::calculateLocalVelocity(unsigned bodyIndex, ds_math::scalar duration) {
	RigidBody* cBody = body[bodyIndex];

	// Cache transform transpose.
	Matrix3 transformTranspose = Matrix3::Transpose(contactToWorld);

	// Work out the velocity in world space
	Vector3 vel = cBody->getVelocity() + Vector3::Cross(cBody->getRotation(), relativeContactPosition[bodyIndex]);

	// Workout velocity in local-space
	Vector3 contactVel = transformTranspose * vel;

	// Workout local acceration
	Vector3 accVel = transformTranspose * (cBody->getLastFrameAcceleration() * duration);

	// Cancel normal axis acceleration
	accVel.x = 0;

	// Apply acceleration to the velocity
	contactVel += accVel;

	return contactVel;
}

void Contact::calculateContactBasis() {
	Vector3 contactTan[2];

	if (abs(contactNormal.x) > abs(contactNormal.y)) {
		scalar s = 1.0/sqrt(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);
		contactTan[0].x =  contactNormal.z*s;
		contactTan[0].y = 0;
		contactTan[0].z = -contactNormal.x*s;

		contactTan[1].x = contactNormal.y*contactTan[0].x;
		contactTan[1].y = contactNormal.z*contactTan[0].x - contactNormal.x*contactTan[0].z;
		contactTan[1].z = -contactNormal.y*contactTan[0].x;
	} else {
		scalar s = 1.0/sqrt(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);
		contactTan[0].x = 0;
		contactTan[0].y = -contactNormal.z*s;
		contactTan[0].z = contactNormal.y*s;

		contactTan[1].x = contactNormal.y*contactTan[0].z - contactNormal.z*contactTan[0].y;
		contactTan[1].y = -contactNormal.y*contactTan[0].z;
		contactTan[1].z = contactNormal.y*contactTan[0].y;
	}

	contactToWorld[0] = contactNormal;
	contactToWorld[1] = contactTan[0];
	contactToWorld[2] = contactTan[1];
}

void Contact::applyImpulse(const ds_math::Vector3& impulse, RigidBody* body, ds_math::Vector3* velocityChange, ds_math::Vector3* rotationChange) {
	///@todo Implement

}

void Contact::applyVelocityChange(ds_math::Vector3 velocityChange[2], ds_math::Vector3 rotationChange[2]) {
	///@todo Implement

}

void Contact::applyPositionChange(ds_math::Vector3 linearChange[2], ds_math::Vector3 angularChange[2], ds_math::scalar penetration) {
	///@todo Implement

}

ds_math::Vector3 Contact::calculateFrictionlessImpulse(ds_math::Matrix3* inverseInertiaTensor) {
	///@todo Implement
	return Vector3();
}

ds_math::Vector3 Contact::calculateFrictionImpulse(ds_math::Matrix3* inverseInertialTensor) {
	///@todo Implement
	return Vector3();
}
