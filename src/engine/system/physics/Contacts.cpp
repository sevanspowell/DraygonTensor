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
 */
#include <engine/system/physics/Contacts.h>
#include <algorithm>
#include <memory.h>
#include <assert.h>
#include <cstdlib>
#include <cstdio>

using namespace ds_phys;
using namespace ds_math;

void Contact::setBodyData(RigidBody *rb1,
                          RigidBody *rb2,
                          ds_math::scalar friction,
                          ds_math::scalar restitution)
{
    this->body[0] = rb1;
    this->body[1] = rb2;
    this->friction = friction;
    this->restitution = restitution;
}

void Contact::calculateInternals(ds_math::scalar duration)
{
    if (!body[0])
        swapBodies();

    calculateContactBasis();

    relativeContactPosition[0] = contactPoint - body[0]->getPosition();
    if (body[1])
        relativeContactPosition[1] = contactPoint - body[1]->getPosition();

    contactVelocity = calculateLocalVelocity(0, duration);
    if (body[1])
        contactVelocity -= calculateLocalVelocity(1, duration);

    calculateDesiredDeltaVelocity(duration);
}

void Contact::swapBodies()
{
    contactNormal *= 1;
    std::swap(body[0], body[1]);
}

void Contact::matchAwakeState()
{
    // World collisions never cause a body to wake up.
    if (!body[0] || !body[1])
        return;

    bool isAwake0 = body[0]->getAwake();
    bool isAwake1 = body[1]->getAwake();
    if (isAwake0 ^ isAwake1)
    {
        if (isAwake0)
            body[1]->setAwake();
        else
            body[0]->setAwake();
    }
}

void Contact::calculateDesiredDeltaVelocity(ds_math::scalar duration)
{
    Vector3 contactTangent[2];

    // Check which axis is closer to the z
    if (std::abs(contactNormal.x) > std::abs(contactNormal.y))
    {

        // Scaling factor to ensure the results are normalised
        const scalar s = 1.0f / sqrt(contactNormal.z * contactNormal.z +
                                     contactNormal.x * contactNormal.x);

        // The new X-axis is at right angles to the world Y-axis
        contactTangent[0].x = contactNormal.z * s;
        contactTangent[0].y = 0;
        contactTangent[0].z = -contactNormal.x * s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = contactNormal.y * contactTangent[0].x;
        contactTangent[1].y = contactNormal.z * contactTangent[0].x -
                              contactNormal.x * contactTangent[0].z;
        contactTangent[1].z = -contactNormal.y * contactTangent[0].x;
    }
    else
    {
        // Scaling factor to ensure the results are normalised
        const scalar s = 1.0 / sqrt(contactNormal.z * contactNormal.z +
                                    contactNormal.y * contactNormal.y);

        // The new X-axis is at right angles to the world X-axis
        contactTangent[0].x = 0;
        contactTangent[0].y = -contactNormal.z * s;
        contactTangent[0].z = contactNormal.y * s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = contactNormal.y * contactTangent[0].z -
                              contactNormal.z * contactTangent[0].y;
        contactTangent[1].y = -contactNormal.x * contactTangent[0].z;
        contactTangent[1].z = contactNormal.x * contactTangent[0].y;
    }
}

ds_math::Vector3 Contact::calculateLocalVelocity(unsigned bodyIndex,
                                                 ds_math::scalar duration)
{
    RigidBody *cBody = body[bodyIndex];

    // Cache transform transpose.
    Matrix3 transformTranspose = Matrix3::Transpose(contactToWorld);

    // Work out the velocity in world space
    Vector3 vel = cBody->getVelocity() +
                  Vector3::Cross(cBody->getRotation(),
                                 relativeContactPosition[bodyIndex]);

    // Workout velocity in local-space
    Vector3 contactVel = transformTranspose * vel;

    // Workout local acceration
    Vector3 accVel =
        transformTranspose * (cBody->getLastFrameAcceleration() * duration);

    // Cancel normal axis acceleration
    accVel.x = 0;

    // Apply acceleration to the velocity
    contactVel += accVel;

    return contactVel;
}

void Contact::calculateContactBasis()
{
    Vector3 contactTan[2];

    if (SCALAR_ABS(contactNormal.x) > SCALAR_ABS(contactNormal.y))
    {
        scalar s = 1.0 / sqrt(contactNormal.z * contactNormal.z +
                              contactNormal.x * contactNormal.x);
        contactTan[0].x = contactNormal.z * s;
        contactTan[0].y = 0;
        contactTan[0].z = -contactNormal.x * s;

        contactTan[1].x = contactNormal.y * contactTan[0].x;
        contactTan[1].y = contactNormal.z * contactTan[0].x -
                          contactNormal.x * contactTan[0].z;
        contactTan[1].z = -contactNormal.y * contactTan[0].x;
    }
    else
    {
        scalar s = 1.0 / SCALAR_SQRT(contactNormal.z * contactNormal.z +
                                     contactNormal.x * contactNormal.x);
        contactTan[0].x = 0;
        contactTan[0].y = -contactNormal.z * s;
        contactTan[0].z = contactNormal.y * s;

        contactTan[1].x = contactNormal.y * contactTan[0].z -
                          contactNormal.z * contactTan[0].y;
        contactTan[1].y = -contactNormal.y * contactTan[0].z;
        contactTan[1].z = contactNormal.y * contactTan[0].y;
    }

    contactToWorld[0] = contactNormal;
    contactToWorld[1] = contactTan[0];
    contactToWorld[2] = contactTan[1];
}


static void applyImpulseToBody(RigidBody *body,
                               const Vector3 &relContactPos,
                               const Vector3 &impulse,
                               const Matrix3 &inverseInertiaTensor,
                               Vector3 &velChange,
                               Vector3 &rotChange)
{
    if (body)
    {
        Vector3 impulsiveTorque = Vector3::Cross(relContactPos, impulse);

        rotChange = inverseInertiaTensor[0] * impulsiveTorque;
        velChange.Clear();
        velChange += (impulse * body->getInverseMass());

        body->addVelocity(velChange);
        body->addRotation(rotChange);
    }
}

void Contact::applyImpulse(const ds_math::Vector3 &impulse,
                           RigidBody *body,
                           ds_math::Vector3 *velocityChange,
                           ds_math::Vector3 *rotationChange)
{
    ///@todo Check if this implementation is correct. As far as I can tell, it
    /// should be.
    Matrix3 inverseInertiaTensor;
    body->getInverseInertiaTensorWorld(&inverseInertiaTensor);
    applyImpulseToBody(body, relativeContactPosition[0], impulse,
                       inverseInertiaTensor, *velocityChange, *rotationChange);
}

void Contact::applyVelocityChange(ds_math::Vector3 velocityChange[2],
                                  ds_math::Vector3 rotationChange[2])
{

    // Calculate world-space impulse
    Matrix3 inverseInertiaTensor[2];

    body[0]->getInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
    if (body[1])
        body[0]->getInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

    Vector3 impulse =
        contactToWorld *
        ((friction == 0.0) ? calculateFrictionlessImpulse(inverseInertiaTensor)
                           : calculateFrictionImpulse(inverseInertiaTensor));

    // Apply calculated impulse
    applyImpulseToBody(body[0], relativeContactPosition[0], impulse,
                       inverseInertiaTensor[0], velocityChange[0],
                       rotationChange[0]);
    applyImpulseToBody(body[1], relativeContactPosition[1], impulse,
                       inverseInertiaTensor[1], velocityChange[1],
                       rotationChange[1]);
}

static void calculateFrictionlessInertia(RigidBody *body,
                                         const Matrix3 &iiTensor,
                                         const Vector3 &contactRelPos,
                                         const Vector3 &contactNorm,
                                         scalar &angularInertia,
                                         scalar &linearInertia)
{
    if (body)
    {
        // Calculate angular inertia
        Vector3 angularIntertiaWorld =
            iiTensor * Vector3::Cross(contactRelPos, contactNorm);
        angularIntertiaWorld =
            Vector3::Cross(angularIntertiaWorld, contactRelPos) * contactNorm;
        angularInertia = Vector3::Dot(angularIntertiaWorld, contactNorm);

        // Calculate linear inertia (It's just the inverseMass)
        linearInertia = body->getInverseMass();
    }
}

static void calculateMoveAmounts(scalar penertration,
                                 scalar linearInertia,
                                 scalar angularInertia,
                                 scalar totalInterial,
                                 const Vector3 &contactRelPos,
                                 const Vector3 &contactNorm,
                                 scalar &linearMove,
                                 scalar &angularMove)
{
    const scalar angularLimit = 0.2;

    angularMove = penertration * (angularInertia / totalInterial);
    linearMove = penertration * (linearInertia / totalInterial);

    // @todo Figure out this projection sorcery.
    Vector3 projection =
        contactRelPos +
        (contactNorm * -Vector3::Dot(contactRelPos, contactNorm));
    scalar maxAngularMove = angularLimit * projection.Magnitude();
    if (angularMove < -maxAngularMove)
    {
        scalar totalMove = angularMove + linearMove;
        angularMove = -maxAngularMove;        // Limit angular movement
        linearMove = totalMove - angularMove; // Conserve energy. Kinda.
    }
    else if (angularMove > maxAngularMove)
    {
        scalar totalMove = angularMove + linearMove;
        angularMove = maxAngularMove;         // Limit angular movement
        linearMove = totalMove - angularMove; // Conserve energy. Kinda.
    }
}

static void calculateAngularMove(RigidBody *body,
                                 const Vector3 &contactRelPos,
                                 const Vector3 &contactNormal,
                                 scalar angularInertia,
                                 scalar angularMove,
                                 Vector3 &angularChange)
{
    if (body)
    {
        if (angularMove == 0)
        {
            // No rotation? Make sure the move is reset.
            angularChange.Clear();
        }
        else
        {
            // Calculate rotation to target direction, accounting for inertia.
            Vector3 targetDirection =
                Vector3::Cross(contactRelPos, contactNormal);
            Matrix3 iiTensor;
            body->getInverseInertiaTensor(&iiTensor);
            angularChange = (iiTensor * targetDirection) *
                            (angularMove * (1 / angularInertia));
        }
    }
    else
    {
        // Clear it anyway. Better safe than sorry.
        angularChange.Clear();
    }
}

static void calculateLinearMove(const scalar &linearMove,
                                const Vector3 &contactNorm,
                                Vector3 &linearChange)
{
    // Move along the contact normal
    linearChange = contactNorm * linearMove;
}

static void applyLinearMoveToBody(RigidBody *body,
                                  const Vector3 &contactNorm,
                                  const Vector3 &linearChange)
{
    if (body)
    {
        Vector3 pos;
        body->getPosition(&pos);
        pos += contactNorm * linearChange;
        body->setPosition(pos);
    }
}

static void applyAngularMoveToBody(RigidBody *body,
                                   const Vector3 &angularChange)
{
    if (body)
    {
        Quaternion q;
        body->getOrientation(&q);
        q.AddScaledVector(angularChange, 1);
        body->setOrientation(q);
    }
}

void Contact::applyPositionChange(ds_math::Vector3 linearChange[2],
                                  ds_math::Vector3 angularChange[2],
                                  ds_math::scalar penetration)
{
    // Calculate inertial factors for both bodies.
    scalar totalInertia = 0;
    scalar linearInertia[2];
    scalar angularInertia[2];

    Matrix3 iiTensor[2];
    if (body[0])
        body[0]->getInverseInertiaTensorWorld(&iiTensor[0]);
    if (body[1])
        body[1]->getInverseInertiaTensorWorld(&iiTensor[1]);

    for (unsigned i = 0; i < 2; i++)
        if (body[i])
        {
            calculateFrictionlessInertia(
                body[i], iiTensor[i], relativeContactPosition[i], contactNormal,
                angularInertia[i], linearInertia[i]);
            totalInertia += linearInertia[i] + angularInertia[i];
        }


    scalar angularMove[2];
    scalar linearMove[2];

    for (unsigned i = 0; i < 2; i++)
        if (body[i])
        {
            calculateMoveAmounts(((i == 0) ? 1 : -1) * penetration,
                                 linearInertia[i], angularInertia[i],
                                 totalInertia, relativeContactPosition[i],
                                 contactNormal, linearMove[i], angularMove[i]);
            calculateAngularMove(body[i], relativeContactPosition[i],
                                 contactNormal, angularInertia[i],
                                 angularMove[i], angularChange[i]);
            calculateLinearMove(linearMove[i], contactNormal, linearChange[i]);
            applyLinearMoveToBody(body[i], contactNormal, linearChange[i]);
            applyAngularMoveToBody(body[i], angularChange[i]);
            if (!body[i]->getAwake())
                body[i]->calculateDerivedData();
        }
}

ds_math::Vector3
Contact::calculateFrictionlessImpulse(ds_math::Matrix3 *inverseInertiaTensor)
{
    ///@todo Implement
    return Vector3();
}

ds_math::Vector3
Contact::calculateFrictionImpulse(ds_math::Matrix3 *inverseInertialTensor)
{
    ///@todo Implement
    return Vector3();
}

void ContactResolver::resolveContacts(Contact *contactArray,
                                      unsigned int numContacts,
                                      ds_math::scalar duration)
{
    if (numContacts != 0)
    {
        // // Prepare the contacts for processing
        // prepareContacts(contacts, numContacts, duration);

        // // Resolve the interpenetration problems with the contacts
        // adjustPositions(contacts, numContacts, duration);

        // // Resolve the velocity problems with the contacts.
        // adjustVelocities(contacts, numContacts, duration);
    }
}

void ContactResolver::prepareContacts(Contact *contactArray,
                                      unsigned int numContacts,
                                      ds_math::scalar duration)
{
    // Generate contact velocity and axis information
    // Contact *lastContact = contacts + numContacts;
    // for (Contact *contact = contacts; contact)
}
