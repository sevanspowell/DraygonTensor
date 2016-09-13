#include <algorithm>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <engine/system/physics/Contacts.h>
#include <memory.h>

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
	const static scalar velocityLimit = 0.25;

	scalar accelVel = 0;

	for(int i = 0; i < 2; i++) {
		if ((body[i]) && (body[i]->getAwake())) {
			accelVel += Vector3::Dot(body[i]->getLastFrameAcceleration() * duration, contactNormal) * ((i%2)?1:-1);
		}
	}

	scalar boundedResitution = restitution;
	if (abs(contactVelocity.x) < velocityLimit) {
		boundedResitution = 0.0;
	}

    desiredDeltaVelocity = -contactVelocity.x - boundedResitution * (contactVelocity.x - accelVel);
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

    if (abs(contactNormal.x) > abs(contactNormal.y))
    {
        scalar s = 1.0 / sqrt(contactNormal.z * contactNormal.z +
                              contactNormal.x * contactNormal.x);
        contactTan[0].x = contactNormal.z * s;
        contactTan[0].y = 0;
        contactTan[0].z = -contactNormal.x * s;

        contactTan[1].x =
            contactNormal.y * contactTan[0].z; // Changed from x to z, saw on
                                               // another branch of project and
                                               // makes sense.
        contactTan[1].y = contactNormal.z * contactTan[0].x -
                          contactNormal.x * contactTan[0].z;
        contactTan[1].z = -contactNormal.y * contactTan[0].x;

        std::cout << "Hello" << std::endl;
    }
    else
    {
        scalar s = 1.0 / sqrt(contactNormal.z * contactNormal.z +
                              contactNormal.y * contactNormal.y);
        contactTan[0].x = 0;
        contactTan[0].y = -contactNormal.z * s;
        contactTan[0].z = contactNormal.y * s;

        contactTan[1].x = contactNormal.y * contactTan[0].z -
                          contactNormal.z * contactTan[0].y;
        contactTan[1].y = -contactNormal.y * contactTan[0].z;
        contactTan[1].z = contactNormal.x * contactTan[0].y;
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

        rotChange = inverseInertiaTensor * impulsiveTorque;
        velChange = (impulse * body->getInverseMass());

        std::cout << "APPLIED TORQUE: " << impulsiveTorque << std::endl;
        std::cout << "APPLIED VELOCITY: " << velChange << std::endl;
        std::cout << "APPLIED ROTATION: " << rotChange << std::endl;

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
    ///should be.
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
    // std::cout << "contactToWorld: " << contactToWorld << std::endl;

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
    else
    {
        angularInertia = 0;
        linearInertia = 0;
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

            //@todo TODO TEMPORARY CODE
            //body[i]->setVelocity(0, 0, 0);
        }
}

ds_math::Vector3
Contact::calculateFrictionlessImpulse(ds_math::Matrix3 *inverseInertiaTensor)
{
    scalar deltaVel = 0;
    scalar angularVel[2];
    scalar linearVel[2];
    for (unsigned i = 0; i < 2; i++)
    {
        calculateFrictionlessInertia(
            body[i], inverseInertiaTensor[i], relativeContactPosition[i],
            contactNormal[i], angularVel[i], linearVel[i]);
        deltaVel += angularVel[i] + linearVel[i];
    }

    if (deltaVel == 0)
        return Vector3(0, 0, 0);
    else
        return Vector3(desiredDeltaVelocity / deltaVel, 0, 0);
}

/**
 * Effectively the same as a vector cross product.
 * We use it for converting betwwen linear and angular values.
 * @param vec
 * @return
 */
static ds_math::Matrix3
calculateSkewSymmetricMatrix(const ds_math::Vector3 &vec)
{
    Matrix3 result;
    result[0] = Vector3(     0, -vec.z,  vec.y);
    result[1] = Vector3( vec.z,      0, -vec.x);
    result[2] = Vector3(-vec.y,  vec.x, 0);

    return result;
}

ds_math::Vector3
Contact::calculateFrictionImpulse(ds_math::Matrix3 *inverseInertiaTensor)
{

    scalar totalInvMass = body[0]->getInverseMass();
    Matrix3 impulseToTorque =
        calculateSkewSymmetricMatrix(relativeContactPosition[0]);
    Matrix3 deltaWorldVel =
        -1 * ((impulseToTorque * inverseInertiaTensor[0]) * impulseToTorque);

    if (body[1])
    {
        Matrix3 impulseToTorque2 =
            calculateSkewSymmetricMatrix(relativeContactPosition[1]);
        deltaWorldVel += -1 * ((impulseToTorque2 * inverseInertiaTensor[1]) *
                               impulseToTorque2);
        totalInvMass += body[1]->getInverseMass();
    }

    // Convert worl velocity to contact-space velocity.
    Matrix3 deltaVelocity =
        (Matrix3::Transpose(contactToWorld) * deltaWorldVel) * contactToWorld;

    // Apply linear velocity change
    deltaVelocity.data[0][0] += totalInvMass;
    deltaVelocity.data[1][1] += totalInvMass;
    deltaVelocity.data[2][2] += totalInvMass;

    // Invert to get the impulse needed per unit of velocity
    Matrix3 impulseMatrix = Matrix3::Inverse(deltaVelocity);

    // Find the target velocities to kill
    Vector3 velKill(desiredDeltaVelocity, -contactVelocity.y,
                    -contactVelocity.z);

    // Find the impulse to kill target velocities
    Vector3 impulseContact = impulseMatrix * velKill;

    // Check for exceeding friction
    scalar planarImpulse = sqrt(impulseContact.y * impulseContact.y +
                                impulseContact.z * impulseContact.z);

    if (planarImpulse > impulseContact.x * friction)
    {
        // We need to use dynamic friction

        scalar invPlanarImpulse = 1 / planarImpulse;
        scalar normalisedYImp = impulseContact.y * invPlanarImpulse;
        scalar normalisedZImp = impulseContact.z * invPlanarImpulse;

        // @todo Figure out what this is about. Appears to be dark magic.
        scalar dynamicFrictionCoeff =
            deltaVelocity.data[0][0] +
            deltaVelocity.data[0][1] * friction * normalisedYImp +
            deltaVelocity.data[0][2] * friction * normalisedZImp;

        impulseContact.x = desiredDeltaVelocity * 1 / dynamicFrictionCoeff;
        impulseContact.y = normalisedYImp * friction * impulseContact.x;
        impulseContact.z = normalisedZImp * friction * impulseContact.x;
    }

    return impulseContact;
}
