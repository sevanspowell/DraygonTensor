#include "engine/system/physics/Contacts.h"

using namespace ds_phys;
using namespace ds_math;

void ContactResolver::resolveContacts(Contact *contactArray,
                                      unsigned numContacts,
                                      scalar duration)
{
    if (numContacts == 0)
        return;
    if (!isValid())
        return;

    prepareContacts(contactArray, numContacts, duration);
    adjustPositions(contactArray, numContacts, duration);
    adjustVelocities(contactArray, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact *contactArray,
                                      unsigned numContacts,
                                      scalar duration)
{
    for (unsigned i = 0; i < numContacts; i++)
    {
        contactArray[i].calculateInternals(duration);
    }
}

void ContactResolver::adjustVelocities(Contact *c,
                                       unsigned numContacts,
                                       scalar duration)
{
    Vector3 velocityChange[2], rotationChange[2];
    Vector3 deltaVel;

    // iteratively handle impacts in order of severity.
    velocityIterationCount = 0;
    while (velocityIterationCount < maxVelocityIterations)
    {
        // Find contact with maximum magnitude of probable velocity change.
        scalar max = velocityEpsilon;
        unsigned index = numContacts;
        for (unsigned i = 0; i < numContacts; i++)
        {
            if (c[i].desiredDeltaVelocity > max)
            {
                max = c[i].desiredDeltaVelocity;
                index = i;
            }
        }
        if (index == numContacts)
            break;

        // Match the awake state at the contact
        c[index].matchAwakeState();

        // Do the resolution on the contact that came out top.
        c[index].applyVelocityChange(velocityChange, rotationChange);

        // With the change in velocity of the two bodies, the update of
        // contact velocities means that some of the relative closing
        // velocities need recomputing.
        for (unsigned cContact = 0; cContact < numContacts; cContact++)
        {
            // Check each body in the contact
            for (unsigned cBody = 0; cBody < 2; cBody++)
            {
                if (c[cContact].body[cBody])
                {
                    // Check for a match with each body in the newly
                    // resolved contact
                    for (unsigned cBody2 = 0; cBody2 < 2; cBody2++)
                    {
                        if (c[cContact].body[cBody] == c[index].body[cBody2])
                        {
                            deltaVel =
                                velocityChange[cBody2] +
                                Vector3::Cross(
                                    rotationChange[cBody2],
                                    c[cContact].relativeContactPosition[cBody]);

                            // The sign of the change is negative if we're
                            // dealing
                            // with the second body in a contact.
                            c[cContact].contactVelocity +=
                                Matrix3::Transpose(c[cContact].contactToWorld) *
                                deltaVel * (cBody ? -1 : 1);
                            c[cContact].calculateDesiredDeltaVelocity(duration);
                        }
                    }
                }
            }
        }
        velocityIterationCount++;
    }
}

void ContactResolver::adjustPositions(Contact *c,
                                      unsigned numContacts,
                                      scalar duration)
{
    unsigned i, index;
    Vector3 linearChange[2], angularChange[2];
    scalar maxPen;
    Vector3 deltaPosition;

    // iteratively resolve interpenetrations in order of severity.
    positionIterationCount = 0;
    while (positionIterationCount < maxPositionIterations)
    {
        // Find biggest penetration
        maxPen = positionEpsilon;
        index = numContacts;
        for (i = 0; i < numContacts; i++)
        {
            if (c[i].penetration > maxPen)
            {
                maxPen = c[i].penetration;
                index = i;
            }
        }
        if (index == numContacts)
            break;

        // Match the awake state at the contact
        c[index].matchAwakeState();

        // Resolve the penetration.
        c[index].applyPositionChange(linearChange, angularChange, maxPen);

        // The resolution may have caused more conflicts.
        // Attempt to resolve them.
        for (i = 0; i < numContacts; i++)
        {
            // Check each body in the contact
            for (unsigned cBody1 = 0; cBody1 < 2; cBody1++)
                if (c[i].body[cBody1])
                {
                    // Check for a match with each body in the newly resolved
                    // contact
                    for (unsigned cBody2 = 0; cBody2 < 2; cBody2++)
                    {
                        if (c[i].body[cBody1] == c[index].body[cBody2])
                        {
                            deltaPosition =
                                linearChange[cBody2] +
                                Vector3::Cross(
                                    angularChange[cBody2],
                                    c[i].relativeContactPosition[cBody1]);

                            // If the body is the second one we need to reverse
                            // the number.
                            c[i].penetration +=
                                Vector3::Dot(deltaPosition,
                                             c[i].contactNormal) *
                                (cBody1 ? 1 : -1);
                        }
                    }
                }
        }
        positionIterationCount++;
    }
}
