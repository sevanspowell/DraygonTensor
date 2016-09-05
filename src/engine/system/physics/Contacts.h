#pragma once

#include <math/Matrix4.h>
#include <math/Precision.h>
#include <math/Vector3.h>
#include <engine/system/physics/RigidBody.h>

namespace ds_phys
{
class ContactResolver;

/**
 * Contains the information relevant about a contact for collision resolution.
 *
 * @remarks It's useful to generate contacts for objects that are nearly colliding, to prevent resolution penetrating them.
 */
class Contact
{
    friend class ContactResolver;

public:
    /**
     * The 2 bodies involved in the contact.
     */
	RigidBody* body[2];

	/**
	 * The lateral friction coefficient
	 */
	ds_math::scalar friction;

	/**
	 * The normal restitution coefficient
	 */
	ds_math::scalar restitution;

	/**
	 * The point of contact
	 */
	ds_math::Vector3 contactPoint;

	/**
	 * The normal of the contact
	 */
	ds_math::Vector3 contactNormal;

	/**
	 * The penetration depth
	 */
	ds_math::scalar penetration;

	/**
	 * Helper method to set the non-contact-specific data.
	 * @param rb1 The first rigid body
	 * @param rb2 The second rigid body
	 * @param friction The friction coefficient
	 * @param restitution The restitution coefficient
	 */
	void setBodyData(RigidBody* rb1, RigidBody* rb2, ds_math::scalar friction, ds_math::scalar restitution);
protected: public:

	/**
	 * The matrix that maps the contact-space to the world-space.
	 * @bug Should be a Matrix3, but we don't have that.
	 */
	ds_math::Matrix3 contactToWorld;

	/**
	 * The closing velocity of the collision.
	 * @remark Set when calculateInternals is called.
	 */
	ds_math::Vector3 contactVelocity;

	/**
	 * The required change in velocity to resolve the contact.
	 */
	ds_math::scalar desiredDeltaVelocity;

	/**
	 * The contact position relative to each of the two bodies in world-space.
	 * @remark Set when calculateInternals is called.
	 */
	ds_math::Vector3 relativeContactPosition[2];

	/**
	 * Updates the internal state of the contact based for the given collision duration.
	 * @param duration The duration of the contact.
	 */
	void calculateInternals(ds_math::scalar duration);

	/**
	 * Swaps the rigid bodies around.
	 * @remarks Does not recalculate internal state.
	 */
	void swapBodies();

	/**
	 * Ensures that both objects are awake, given that atleast one of the bodies already is.
	 */
	void matchAwakeState();

	/**
	 * Calculates the desired change in velocity for the contact resolution.
	 * @param duration The direction of the collision.
	 */
	void calculateDesiredDeltaVelocity(ds_math::scalar duration);

	/**
	 * Calculates and returns the velocity of the contact point, for the given body index.
	 * @param bodyIndex The index of the body
	 * @param duration The duration of the collison
	 * @pre bodyIndex must be in the range [0, 1]
	 * @return The velocity o the contact point.
	 */
	ds_math::Vector3 calculateLocalVelocity(unsigned bodyIndex, ds_math::scalar duration);

	/**
	 * Calculates the collision space for anisotropic or isotropic friction
	 */
	void calculateContactBasis();

	/**
	 * Applies an impulse to the given body. Returning the change in rotational and linear velocity.
	 * @param impulse The impulse force to apply.
	 * @param body The rigid body to apply the force too.
	 * @param velocityChange (out) The change in linear velocity of the rigid body
	 * @param rotationChange (out) The change in rotational velocity of the rigid body
	 */
	void applyImpulse(const ds_math::Vector3& impulse, RigidBody* body, ds_math::Vector3* velocityChange, ds_math::Vector3* rotationChange);

	/**
	 * Applies an inertia-weighted impulse to the rigid bodies to resolve the contact.
	 * @param velocityChange (out) The change in linear velocity for each object.
	 * @param rotationChange (out) The change in rotational velocity for each object.
	 *
	 * @todo Investigate if it's worth changing the arrays to discrete pointers.
	 */
	void applyVelocityChange(ds_math::Vector3 velocityChange[2], ds_math::Vector3 rotationChange[2]);

	/**
	 *	Applies a translation to each rigid body to resolve the contact.
	 * @param linearChange (out) The change in linear velocity.
	 * @param angularChange (out) The change in rotational velocity.
	 * @param penetration The depth of the penetration to resolve.
	 * @todo Verify "penetration" description is accurate.
	 */
	void applyPositionChange(ds_math::Vector3 linearChange[2], ds_math::Vector3 angularChange[2], ds_math::scalar penetration);

	/**
	 * Calculates the impulse required to resolve the contact, assuming there is not friction.
	 * @param inverseInertiaTensor Each inertia tensor for the rigid bodies.
	 * @return The required impulse
	 */
	ds_math::Vector3 calculateFrictionlessImpulse(ds_math::Matrix3* inverseInertiaTensor);

	/**
	 * Calculates the impulse required to resolve the contact, assuming there is friction.
	 * @param inverseInertiaTensor Each inertia tensor for the rigid bodies.
	 * @return The required impulse
	 */
	ds_math::Vector3 calculateFrictionImpulse(ds_math::Matrix3* inverseInertialTensor);
};

class ContactResolver
{
};

class IContactGenerator
{
public:
    /**
     * Fills the contact array with contacts generated by the contact generator.
     * The contact pointer should point to the first available contact in the
     * contact array, where limit is the maximum number of contacts in the array
     * that can be written to. The method returns the number of contacts that
     * actually have been written to.
     *
     * @param   contact   Contact *, pointer to first contact in array.
     * @param   limit     unsigned int, size of the contact array.
     * @return            unsigned int, number of contacts that were actually
     *                    written to.
     */
    virtual unsigned int addContact(Contact *contact,
                                    unsigned int limit) const = 0;
};
}
