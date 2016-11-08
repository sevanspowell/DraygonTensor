#ifndef ENGINE_SYSTEM_PHYSICS_CONTACTRESOLVER_H_
#define ENGINE_SYSTEM_PHYSICS_CONTACTRESOLVER_H_

#include "engine/system/physics/Contacts.h"
#include "math/Precision.h"

namespace ds_phys
{
/**
 * Contact resolver class.
 */
class ContactResolver
{
private:
    /// If the internal state needs to be updated.
    // bool isDirty;

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


    ContactResolver(unsigned maxIterations,
                    ds_math::scalar velEpsilon = 0.000001,
                    ds_math::scalar posEpsilon = 0.000001)
        : maxVelocityIterations(maxIterations),
          maxPositionIterations(maxIterations),
          velocityEpsilon(velEpsilon),
          positionEpsilon(posEpsilon),
          velocityIterationCount(0),
          positionIterationCount(0)
    {
    }

    ContactResolver(unsigned maxVelIterations,
                    unsigned maxPosIterations,
                    ds_math::scalar velEpsilon = 0.000001,
                    ds_math::scalar posEpsilon = 0.000001)
        : maxVelocityIterations(maxVelIterations),
          maxPositionIterations(maxPosIterations),
          velocityEpsilon(velEpsilon),
          positionEpsilon(posEpsilon),
          velocityIterationCount(0),
          positionIterationCount(0)
    {
    }

    /**
     * Returns if the contact resolver is in a valid state.
     * @return If the contact resolver is in a valid state.
     */
    bool isValid()
    {
        return (maxVelocityIterations > 0) && (maxPositionIterations > 0) &&
               (velocityEpsilon >= 0) && (positionEpsilon >= 0);
    }

    /**
     * Sets the max number of iterations for each resolution stage.
     * @param maxVel The max number of velocity iterations
     * @param maxPos The max number of position iterations
     */
    void setIterations(unsigned maxVel, unsigned maxPos)
    {
        maxVelocityIterations = maxVel;
        maxPositionIterations = maxPos;
    }

    /**
     * Sets the max number of iterations for both resolution stages.
     * @param iters The max number of iterations for both stages.
     */
    void setIterations(unsigned iters)
    {
        setIterations(iters, iters);
    }

    /**
     * Sets the tolerance value for both velocity and position.
     * @param velocityEpsilon The epsilon for velocity.
     * @param positionEpsilon The epsilon for position.
     */
    void setEpsilon(ds_math::scalar velEpsilon, ds_math::scalar posEpsilon)
    {
        velocityEpsilon = velEpsilon;
        positionEpsilon = posEpsilon;
    }

    /**
     * Resolves a set of contacts for both penetration and velocity.
     *
     * @param contactArray Pointer to an array of contact objects.
     * @param numContacts The number of contacts in the array to resolve.
     * @param numIterations Max number of iterations to perform. Reduces worst
     * case performance.
     * @param duration The duration of the previous integration step.
     *
     * @remark Related collisions should be batched together to improve
     * performance.
     */
    void resolveContacts(Contact *contactArray,
                         unsigned numContacts,
                         ds_math::scalar duration);


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
    void prepareContacts(Contact *contactArray,
                         unsigned numContacts,
                         ds_math::scalar duration);

    /**
     * Resolves the velocity issues with the given array of constraints,
     * using the given number of iterations.
     *
     * @param contactArray The contacts to solve velocities for.
     * @param numContacts The number of contacts
     * @param duration Duration of the physics frame.
     */
    void adjustVelocities(Contact *contactArray,
                          unsigned numContacts,
                          ds_math::scalar duration);

    /**
     * Resolves the positional issues with the given array of constraints,
     * using the given number of iterations.
     *
     * @param contactArray The contacts to solve positions for.
     * @param numContacts The number of contacts
     * @param duration Duration of the physics frame.
     */
    void adjustPositions(Contact *contacts,
                         unsigned numContacts,
                         ds_math::scalar duration);
};
}

#endif
