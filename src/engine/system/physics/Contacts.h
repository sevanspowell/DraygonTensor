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
	private:
		/// If the internal state needs to be updated.
		bool isDirty;

		/// The max number of velocity iterations to attempt.
		unsigned maxVelocityIterations;

		/// The max number of position iterations to attempt.
		unsigned maxPositionIterations;

		/// Ignore anything smaller than this epsilon to promote velocity stability.
		ds_math::scalar velocityEpsilon;

		/// Ignore anything smaller than this epsilon to promote position stability.
		ds_math::scalar positionEpsilon;

	public:
		/// Current number of iterations for velocity resolution.
		unsigned velocityIterationCount;

		/// Current number of iterations for position resolution.
		unsigned positionIterationCount;


		ContactResolver(unsigned maxIterations, ds_math::scalar velEpsilon = 0.01, ds_math::scalar posEpsilon = 0.01)
			: isDirty(true), maxVelocityIterations(maxIterations), maxPositionIterations(maxIterations), velocityEpsilon(velEpsilon), positionEpsilon(posEpsilon),
			  velocityIterationCount(0), positionIterationCount(0) {}

		ContactResolver(unsigned maxVelIterations, unsigned maxPosIterations, ds_math::scalar velEpsilon = 0.01, ds_math::scalar posEpsilon = 0.01)
			: isDirty(true), maxVelocityIterations(maxVelIterations), maxPositionIterations(maxPosIterations), velocityEpsilon(velEpsilon), positionEpsilon(posEpsilon),
			  velocityIterationCount(0), positionIterationCount(0) {}

		/**
		 * Returns if the contact resolver is in a valid state.
		 * @return If the contact resolver is in a valid state.
		 */
		bool isValid() {
			return (maxVelocityIterations > 0)
			    && (maxPositionIterations > 0)
			    && (velocityEpsilon > 0)
			    && (positionEpsilon > 0);
		}

		/**
		 * Sets the max number of iterations for each resolution stage.
		 * @param maxVel The max number of velocity iterations
		 * @param maxPos The max number of position iterations
		 */
		void setIterations(unsigned maxVel, unsigned maxPos) {
			maxVelocityIterations = maxVel;
			maxPositionIterations = maxPos;
		}

		/**
		 * Sets the max number of iterations for both resolution stages.
		 * @param iters The max number of iterations for both stages.
		 */
		void setIterations(unsigned iters) {
			setIterations(iters, iters);
		}

	    /**
	     * Sets the tolerance value for both velocity and position.
	     * @param velocityEpsilon The epsilon for velocity.
	     * @param positionEpsilon The epsilon for position.
	     */
	    void setEpsilon(ds_math::scalar velEpsilon, ds_math::scalar posEpsilon) {
	    	velocityEpsilon = velEpsilon;
	    	positionEpsilon = posEpsilon;
	    }

	    /**
	     * Resolves a set of contacts for both penetration and velocity.
	     *
	     * @param contactArray Pointer to an array of contact objects.
	     * @param numContacts The number of contacts in the array to resolve.
	     * @param numIterations Max number of iterations to perform. Reduces worst case performance.
	     * @param duration The duration of the previous integration step.
	     *
	     * @remark Related collisions should be batched together to improve performance.
	     */
	    void resolveContacts(Contact *contactArray, unsigned numContacts, ds_math::scalar duration);


	protected:
	    /**
	     * Sets up contacts ready for processing. This makes sure their
	     * internal data is configured correctly and the correct set of bodies
	     * is made alive.
	     *
	     * @param contactArray The contacts to prepare.
	     * @param numContacts The number of contacts
	     * @param duration Duration of the physics frame.
	     */
	    void prepareContacts(Contact *contactArray, unsigned numContacts, ds_math::scalar duration);

	    /**
	     * Resolves the velocity issues with the given array of constraints,
	     * using the given number of iterations.
	     *
	     * @param contactArray The contacts to solve velocities for.
	     * @param numContacts The number of contacts
	     * @param duration Duration of the physics frame.
	     */
	    void adjustVelocities(Contact *contactArray, unsigned numContacts, ds_math::scalar duration);

	    /**
	     * Resolves the positional issues with the given array of constraints,
	     * using the given number of iterations.
	     *
	     * @param contactArray The contacts to solve positions for.
	     * @param numContacts The number of contacts
	     * @param duration Duration of the physics frame.
	     */
	    void adjustPositions(Contact *contacts, unsigned numContacts, ds_math::scalar duration);
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
