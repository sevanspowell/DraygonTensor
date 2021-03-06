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
#include <algorithm>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory.h>

#include <engine/system/physics/CollisionFine.h>

using namespace ds_phys;

CollisionPrimitive::CollisionPrimitive() : body(nullptr)
{
}

void CollisionPrimitive::calculateInternals()
{
    if (body != nullptr)
    {
        transform = body->getTransform() * offset;
    }
}

bool IntersectionTests::sphereAndHalfSpace(const CollisionSphere &sphere,
                                           const CollisionPlane &plane)
{
    // // Check for the intersection
    return (ds_math::Vector3::Dot(plane.direction, sphere.getAxis(3)) +
            sphere.radius) <= plane.offset;
}

// bool IntersectionTests::sphereAndSphere(const CollisionSphere &one,
//                                        const CollisionSphere &two)
//{
//    // // Find the vector between the objects
//    // Vector3 midline = one.getAxis(3) - two.getAxis(3);
//
//    // // See if it is large enough.
//    // return ds_math::Vector3::Dot(midline, midline) <
//    //     (one.radius+two.radius)*(one.radius+two.radius);
//    return false;
//}

static inline ds_math::scalar transformToAxis(const CollisionBox &box,
                                              const ds_math::Vector3 &axis)
{

    return box.halfSize.x * fabs(ds_math::Vector3::Dot(axis, box.getAxis(0))) +
           box.halfSize.y * fabs(ds_math::Vector3::Dot(axis, box.getAxis(1))) +
           box.halfSize.z * fabs(ds_math::Vector3::Dot(axis, box.getAxis(2)));
    // return (ds_math::Vector3::Dot(axis,
    // ds_math::Matrix3(box.getTransform()[0], box.getTransform()[1],
    // box.getTransform()[2])*box.halfSize));
}

/**
 * This function checks if the two boxes overlap
 * along the given axis. The final parameter toCentre
 * is used to pass in the vector between the boxes centre
 * points, to avoid having to recalculate it each time.
 */
static inline bool overlapOnAxis(const CollisionBox &one,
                                 const CollisionBox &two,
                                 ds_math::Vector3 axis,
                                 const ds_math::Vector3 &toCentre)
{
    // if (ds_math::Vector3::Dot(axis, axis) < 0.0001) return true;

    // Project the half-size of one onto axis
    ds_math::scalar oneProject = transformToAxis(one, axis);
    ds_math::scalar twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    ds_math::scalar distance = fabs(ds_math::Vector3::Dot(toCentre, axis));

    // Check for overlap
    return (distance < oneProject + twoProject);
}

bool IntersectionTests::capsuleAndHalfSpace(const CollisionCapsule &cap,
                                            const CollisionPlane &plane)
{
    // Sphere 1
    {
        // SphereOrigin1 = BodyOrigin + UpVec scaled by half height
        auto sphereOrigin =
            cap.getAxis(3) + cap.getAxis(1) * (cap.height / 2.0);

        if ((ds_math::Vector3::Dot(plane.direction, sphereOrigin) - cap.radius -
             plane.offset) <= 0)
        {
            return true;
        }
    }

    // Sphere 2
    {
        // SphereOrigin2 = BodyOrigin - UpVec scaled by half height
        auto sphereOrigin =
            cap.getAxis(3) - cap.getAxis(1) * (cap.height / 2.0);

        if ((ds_math::Vector3::Dot(plane.direction, sphereOrigin) - cap.radius -
             plane.offset) <= 0)
        {
            return true;
        }
    }

    // Cylinder Collision doesn't need to be considered as either sphere will
    // take care of it.
    return false;
}

// This preprocessor definition is only used as a convenience
// in the boxAndBox intersection  method.
#define TEST_OVERLAP(axis) overlapOnAxis(one, two, (axis), toCentre)

bool IntersectionTests::boxAndBox(const CollisionBox &one,
                                  const CollisionBox &two)
{
    // Find the vector between the two centres
    ds_math::Vector3 toCentre = two.getAxis(3) - one.getAxis(3);


    return (
        // Check on box one's axes first
        TEST_OVERLAP(one.getAxis(0)) && TEST_OVERLAP(one.getAxis(1)) &&
        TEST_OVERLAP(one.getAxis(2)) &&

        // And on two's
        TEST_OVERLAP(two.getAxis(0)) && TEST_OVERLAP(two.getAxis(1)) &&
        TEST_OVERLAP(two.getAxis(2)) &&

        // Now on the cross products
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(0))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(1))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(2))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(0))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(1))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(2))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(0))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(1))) &&
        TEST_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(2))));
}
#undef TEST_OVERLAP

bool IntersectionTests::boxAndHalfSpace(const CollisionBox &box,
                                        const CollisionPlane &plane)
{
    // Work out the projected radius of the box onto the plane direction
    ds_math::scalar projectedRadius = transformToAxis(box, plane.direction);

    // Work out how far the box is from the origin
    ds_math::scalar boxDistance =
        ds_math::Vector3::Dot(plane.direction, box.getAxis(3)) -
        projectedRadius;

    // Check for the intersection
    return boxDistance <= plane.offset;
}

unsigned CollisionDetector::sphereAndHalfSpace(const CollisionSphere &sphere,
                                               const CollisionPlane &plane,
                                               CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    // Cache the sphere position
    ds_math::Vector3 position = sphere.getAxis(3);

    // Find the distance from the plane
    ds_math::scalar ballDistance =
        ds_math::Vector3::Dot(plane.direction, position) - sphere.radius -
        plane.offset;

    if (ballDistance >= 0)
        return 0;

    // Create the contact - it has a normal in the plane direction.
    Contact *contact = data->contacts;
    contact->contactNormal = plane.direction;
    contact->penetration = -ballDistance;
    contact->contactPoint =
        position - plane.direction * (ballDistance + sphere.radius);


    contact->setBodyData(
        sphere.body ? (sphere.body->getMass() == 0 ? nullptr : sphere.body)
                    : (nullptr),
        plane.body ? (plane.body->getMass() == 0 ? nullptr : plane.body)
                   : (nullptr),
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::sphereAndSphere(const CollisionSphere &one,
                                            const CollisionSphere &two,
                                            CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;
    if ((((one.body) && (one.body->getMass() == 0)) || !(one.body)) &&
        (((two.body) && (two.body->getMass() == 0)) || !(two.body)))
        return 0;

    // Cache the sphere positions
    ds_math::Vector3 positionOne = one.getAxis(3);
    ds_math::Vector3 positionTwo = two.getAxis(3);

    // Find the vector between the objects
    ds_math::Vector3 midline = positionOne - positionTwo;
    ds_math::scalar size = midline.Magnitude();

    // See if it is large enough.
    if (size <= 0.0f || size >= one.radius + two.radius)
    {
        return 0;
    }

    // We manually create the normal, because we have the
    // size to hand.
    ds_math::Vector3 normal = midline * ((1.0) / size);

    Contact *contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = positionOne + midline * 0.5;
    contact->penetration = (one.radius + two.radius - size);

    contact->setBodyData(
        one.body ? (one.body->getMass() == 0 ? nullptr : one.body) : (nullptr),
        two.body ? (two.body->getMass() == 0 ? nullptr : two.body) : (nullptr),
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}


/*
 * This function checks if the two boxes overlap
 * along the given axis, returning the ammount of overlap.
 * The final parameter toCentre
 * is used to pass in the vector between the boxes centre
 * points, to avoid having to recalculate it each time.
 */
static inline ds_math::scalar
penetrationOnAxis(const CollisionBox &one,
                  const CollisionBox &two,
                  const ds_math::Vector3 &axis,
                  const ds_math::Vector3 &toCentre)
{
    // Project the half-size of one onto axis
    ds_math::scalar oneProject = transformToAxis(one, axis);
    ds_math::scalar twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    ds_math::scalar distance = fabs(ds_math::Vector3::Dot(toCentre, axis));

    // Return the overlap (i.e. positive indicates
    // overlap, negative indicates separation).
    return oneProject + twoProject - distance;
}


static inline bool tryAxis(const CollisionBox &one,
                           const CollisionBox &two,
                           ds_math::Vector3 axis,
                           const ds_math::Vector3 &toCentre,
                           unsigned index,

                           // These values may be updated
                           ds_math::scalar &smallestPenetration,
                           unsigned &smallestCase)
{
    // Make sure we have a normalized axis, and don't check almost parallel axes
    if (ds_math::Vector3::Dot(axis, axis) < 0.0001)
        return true;
    axis.Normalize();

    ds_math::scalar penetration = penetrationOnAxis(one, two, axis, toCentre);

    if (penetration < 0)
        return false;
    if (penetration < smallestPenetration)
    {
        smallestPenetration = penetration;
        smallestCase = index;
    }
    return true;
}

void fillPointFaceBoxBox(const CollisionBox &one,
                         const CollisionBox &two,
                         const ds_math::Vector3 &toCentre,
                         CollisionData *data,
                         unsigned best,
                         ds_math::scalar pen)
{
    // This method is called when we know that a vertex from
    // box two is in contact with box one.

    Contact *contact = data->contacts;

    // We know which axis the collision is on (i.e. best),
    // but we need to work out which of the two faces on
    // this axis.
    ds_math::Vector3 normal = one.getAxis(best);
    if (ds_math::Vector3::Dot(one.getAxis(best), toCentre) > 0)
    {
        normal = normal * -1.0f;
    }

    // Work out which vertex of box two we're colliding with.
    // Using toCentre doesn't work!
    ds_math::Vector3 vertex = two.halfSize;
    if (ds_math::Vector3::Dot(two.getAxis(0), normal) < 0)
        vertex.x = -vertex.x;
    if (ds_math::Vector3::Dot(two.getAxis(1), normal) < 0)
        vertex.y = -vertex.y;
    if (ds_math::Vector3::Dot(two.getAxis(2), normal) < 0)
        vertex.z = -vertex.z;

    // Create the contact data
    contact->contactNormal = normal;
    contact->penetration = pen;
    contact->contactPoint = two.getTransform() * vertex;

    contact->setBodyData(
        one.body ? (one.body->getMass() == 0 ? nullptr : one.body) : (nullptr),
        two.body ? (two.body->getMass() == 0 ? nullptr : two.body) : (nullptr),
        data->friction, data->restitution);
}

static inline ds_math::Vector3
contactPoint(const ds_math::Vector3 &pOne,
             const ds_math::Vector3 &dOne,
             ds_math::scalar oneSize,
             const ds_math::Vector3 &pTwo,
             const ds_math::Vector3 &dTwo,
             ds_math::scalar twoSize,

             // If this is true, and the contact point is outside
             // the edge (in the case of an edge-face contact) then
             // we use one's midpoint, otherwise we use two's.
             bool useOne)
{
    ds_math::Vector3 toSt, cOne, cTwo;
    ds_math::scalar dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    ds_math::scalar denom, mua, mub;

    smOne = ds_math::Vector3::Dot(dOne, dOne);
    smTwo = ds_math::Vector3::Dot(dOne, dOne);
    dpOneTwo = ds_math::Vector3::Dot(dTwo, dOne);

    toSt = pOne - pTwo;
    dpStaOne = ds_math::Vector3::Dot(dOne, toSt);
    dpStaTwo = ds_math::Vector3::Dot(dTwo, toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    // Zero denominator indicates parrallel lines
    if (fabs(denom) < 0.0001f)
    {
        return useOne ? pOne : pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    // If either of the edges has the nearest point out
    // of bounds, then the edges aren't crossed, we have
    // an edge-face contact. Our point is on the edge, which
    // we know from the useOne parameter.
    if (mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
    {
        return useOne ? pOne : pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}

// This preprocessor definition is only used as a convenience
// in the boxAndBox contact generation method.
#define CHECK_OVERLAP(axis, index)                                             \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best))              \
        return 0;

unsigned CollisionDetector::boxAndBox(const CollisionBox &one,
                                      const CollisionBox &two,
                                      CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((one.body) && (one.body->getMass() == 0)) || !(one.body)) &&
        (((two.body) && (two.body->getMass() == 0)) || !(two.body)))
        return 0;

    if (!IntersectionTests::boxAndBox(one, two))
        return 0;

    // Find the vector between the two centres
    ds_math::Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

    // We start assuming there is no contact
    ds_math::scalar pen = ds_math::SCALAR_MAX;
    unsigned best = 0xffffff;

    // Now we check each axes, returning if it gives us
    // a separating axis, and keeping track of the axis with
    // the smallest penetration otherwise.
    CHECK_OVERLAP(one.getAxis(0), 0);
    CHECK_OVERLAP(one.getAxis(1), 1);
    CHECK_OVERLAP(one.getAxis(2), 2);

    CHECK_OVERLAP(two.getAxis(0), 3);
    CHECK_OVERLAP(two.getAxis(1), 4);
    CHECK_OVERLAP(two.getAxis(2), 5);

    // Store the best axis-major, in case we run into almost
    // parallel edge collisions later
    unsigned bestSingleAxis = best;

    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(0)), 6);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(1)), 7);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(0), two.getAxis(2)), 8);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(0)), 9);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(1)), 10);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(1), two.getAxis(2)), 11);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(0)), 12);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(1)), 13);
    CHECK_OVERLAP(ds_math::Vector3::Cross(one.getAxis(2), two.getAxis(2)), 14);

    // Make sure we've got a result.
    assert(best != 0xffffff);

    // We now know there's a collision, and we know which
    // of the axes gave the smallest penetration. We now
    // can deal with it in different ways depending on
    // the case.
    if (best < 3)
    {
        // We've got a vertex of box two on a face of box one.
        fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
        data->addContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        // We've got a vertex of box one on a face of box two.
        // We use the same algorithm as above, but swap around
        // one and two (and therefore also the vector between their
        // centres).
        fillPointFaceBoxBox(two, one, toCentre * -1.0f, data, best - 3, pen);
        data->addContacts(1);
        return 1;
    }
    else
    {
        // We've got an edge-edge contact. Find out which axes
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        ds_math::Vector3 oneAxis = one.getAxis(oneAxisIndex);
        ds_math::Vector3 twoAxis = two.getAxis(twoAxisIndex);
        ds_math::Vector3 axis = ds_math::Vector3::Cross(oneAxis, twoAxis);
        axis.Normalize();

        // The axis should point from box one to box two.
        {
            if (ds_math::Vector3::Dot(axis, toCentre) > 0)
                axis = axis * -1.0f;
        }
        // We have the axes, but not the edges: each axis has 4 edges parallel
        // to it, we need to find which of the 4 for each object. We do
        // that by finding the point in the centre of the edge. We know
        // its component in the direction of the box's collision axis is zero
        // (its a mid-point) and we determine which of the extremes in each
        // of the other axes is closest.
        ds_math::Vector3 ptOnOneEdge = one.halfSize;
        ds_math::Vector3 ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; i++)
        {
            if (i == oneAxisIndex)
                ptOnOneEdge[i] = 0;
            else if (ds_math::Vector3::Dot(one.getAxis(i), axis) > 0)
                ptOnOneEdge[i] = -ptOnOneEdge[i];

            if (i == twoAxisIndex)
                ptOnTwoEdge[i] = 0;
            else if (ds_math::Vector3::Dot(two.getAxis(i), axis) < 0)
                ptOnTwoEdge[i] = -ptOnTwoEdge[i];
        }

        // Move them into world coordinates (they are already oriented
        // correctly, since they have been derived from the axes).
        ptOnOneEdge = one.transform * ptOnOneEdge;
        ptOnTwoEdge = two.transform * ptOnTwoEdge;

        // So we have a point and a direction for the colliding edges.
        // We need to find out point of closest approach of the two
        // line-segments.
        ds_math::Vector3 vertex = contactPoint(
            ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex], ptOnTwoEdge,
            twoAxis, two.halfSize[twoAxisIndex], bestSingleAxis > 2);

        // We can fill the contact.
        Contact *contact = data->contacts;

        contact->penetration = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;

        contact->setBodyData(
            one.body ? (one.body->getMass() == 0 ? nullptr : one.body)
                     : (nullptr),
            two.body ? (two.body->getMass() == 0 ? nullptr : two.body)
                     : (nullptr),
            data->friction, data->restitution);

        data->addContacts(1);

        return 1;
    }
    return 0;
}
#undef CHECK_OVERLAP


unsigned CollisionDetector::boxAndPoint(const CollisionBox &box,
                                        const ds_math::Vector3 &point,
                                        CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((box.body) && (box.body->getMass() == 0)) || !(box.body)))
        return 0;

    // Transform the point into box coordinates
    ds_math::Vector3 relPt = ds_math::Matrix4::Inverse(box.transform) * point;

    ds_math::Vector3 normal;

    // Check each axis, looking for the axis on which the
    // penetration is least deep.
    ds_math::scalar min_depth = box.halfSize.x - fabs(relPt.x);
    if (min_depth < 0)
        return 0;
    normal = box.getAxis(0) * ((relPt.x < 0) ? -1 : 1);

    ds_math::scalar depth = box.halfSize.y - fabs(relPt.y);
    if (depth < 0)
        return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * ((relPt.y < 0) ? -1 : 1);
    }

    depth = box.halfSize.z - fabs(relPt.z);
    if (depth < 0)
        return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * ((relPt.z < 0) ? -1 : 1);
    }

    // Compile the contact
    Contact *contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

    // Note that we don't know what rigid body the point
    // belongs to, so we just use NULL. Where this is called
    // this value can be left, or filled in.
    contact->setBodyData(
        box.body ? (box.body->getMass() == 0 ? nullptr : box.body) : (nullptr),
        (nullptr), data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndSphere(const CollisionBox &box,
                                         const CollisionSphere &sphere,
                                         CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((box.body) && (box.body->getMass() == 0)) || !(box.body)) &&
        (((sphere.body) && (sphere.body->getMass() == 0)) || !(sphere.body)))
        return 0;

    // Transform the centre of the sphere into box coordinates
    ds_math::Vector3 centre = sphere.getAxis(3);
    ds_math::Vector3 relCentre =
        ds_math::Matrix4::Inverse(box.transform) * centre;

    // Early out check to see if we can exclude the contact
    if (fabs(relCentre.x) - sphere.radius > box.halfSize.x ||
        fabs(relCentre.y) - sphere.radius > box.halfSize.y ||
        fabs(relCentre.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    ds_math::Vector3 closestPt(0, 0, 0);
    ds_math::scalar dist;

    // Clamp each coordinate to the box.
    dist = relCentre.x;
    if (dist > box.halfSize.x)
        dist = box.halfSize.x;
    if (dist < -box.halfSize.x)
        dist = -box.halfSize.x;
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.halfSize.y)
        dist = box.halfSize.y;
    if (dist < -box.halfSize.y)
        dist = -box.halfSize.y;
    closestPt.y = dist;

    dist = relCentre.z;
    if (dist > box.halfSize.z)
        dist = box.halfSize.z;
    if (dist < -box.halfSize.z)
        dist = -box.halfSize.z;
    closestPt.z = dist;

    // Check we're in contact
    dist =
        ds_math::Vector3::Dot((closestPt - relCentre), (closestPt - relCentre));
    if (dist > sphere.radius * sphere.radius)
        return 0;

    // Compile the contact
    ds_math::Vector3 closestPtWorld = box.transform * closestPt;

    Contact *contact = data->contacts;
    contact->contactNormal = (closestPtWorld - centre);
    if (contact->contactNormal == ds_math::Vector3(0, 0, 0))
        return 0;
    contact->contactNormal =
        ds_math::Vector3::Normalize(contact->contactNormal);
    contact->contactPoint = closestPtWorld;
    contact->penetration = sphere.radius - sqrt(dist);

    contact->setBodyData(
        box.body ? (box.body->getMass() == 0 ? nullptr : box.body) : (nullptr),
        sphere.body ? (sphere.body->getMass() == 0 ? nullptr : sphere.body)
                    : (nullptr),
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(const CollisionBox &box,
                                            const CollisionPlane &plane,
                                            CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((box.body) && (box.body->getMass() == 0)) || !(box.body)) &&
        (((plane.body) && (plane.body->getMass() == 0)) || !(plane.body)))
        return 0;

    // Check for intersection
    if (!IntersectionTests::boxAndHalfSpace(box, plane))
    {
        return 0;
    }

    // We have an intersection, so find the intersection points. We can make
    // do with only checking vertices. If the box is resting on a plane
    // or on an edge, it will be reported as four or two contact points.

    // Go through each combination of + and - for each half-size
    static ds_math::scalar mults[8][3] = {{1, 1, 1},   {-1, 1, 1},  {1, -1, 1},
                                          {-1, -1, 1}, {1, 1, -1},  {-1, 1, -1},
                                          {1, -1, -1}, {-1, -1, -1}};

    Contact *contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++)
    {

        // Calculate the position of each vertex
        ds_math::Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos = vertexPos * box.halfSize; // was componentProductUpdate
        vertexPos = ds_math::Matrix4::Transform(box.transform, vertexPos);

        // Calculate the distance from the plane
        ds_math::scalar vertexDistance =
            ds_math::Vector3::Dot(vertexPos, plane.direction);

        // Compare this to the plane's distance
        if (vertexDistance <= plane.offset)
        {
            // Create the contact data.

            // The contact point is halfway between the vertex and the
            // plane - we multiply the direction by half the separation
            // distance and add the vertex location.
            // @todo TODO Should this be 0.5f? (Doesn't seem to matter afaik)
            // contact->contactPoint =
            //     vertexPos +
            //     1.0f * (plane.direction *
            //             (std::abs(vertexDistance - plane.offset)));
            contact->contactPoint = plane.direction;
            contact->contactPoint =
                contact->contactPoint * (vertexDistance - plane.offset);
            contact->contactPoint = contact->contactPoint + vertexPos;

            contact->contactNormal = plane.direction;
            contact->penetration = (plane.offset - vertexDistance);

            // Write the appropriate data
            contact->setBodyData(
                box.body ? (box.body->getMass() == 0 ? nullptr : box.body)
                         : (nullptr),
                plane.body ? (plane.body->getMass() == 0 ? nullptr : plane.body)
                           : (nullptr),
                data->friction, data->restitution);

            // Move onto the next contact
            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned)data->contactsLeft)
                return contactsUsed;
        }
    }

    data->addContacts(contactsUsed);
    return contactsUsed;
}

unsigned CollisionDetector::capsuleAndHalfSpace(const CollisionCapsule &cap,
                                                const CollisionPlane &plane,
                                                CollisionData *data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((cap.body) && (cap.body->getMass() == 0)) || !(cap.body)) &&
        (((plane.body) && (plane.body->getMass() == 0)) || !(plane.body)))
        return 0;

    if (!IntersectionTests::capsuleAndHalfSpace(cap, plane))
    {
        return 0;
    }

    auto sphereOrigin1 = cap.getAxis(3) + cap.getAxis(1) * (cap.height / 2.0);
    auto sphereOrigin2 = cap.getAxis(3) - cap.getAxis(1) * (cap.height / 2.0);
    auto sphereDist1 = (ds_math::Vector3::Dot(plane.direction, sphereOrigin1) -
                        cap.radius - plane.offset);
    auto sphereDist2 = (ds_math::Vector3::Dot(plane.direction, sphereOrigin2) -
                        cap.radius - plane.offset);

    auto sphereOrigin =
        sphereDist2 > sphereDist1 ? sphereOrigin1 : sphereOrigin2;
    auto sphereDist = sphereDist2 > sphereDist1 ? sphereDist1 : sphereDist2;
    auto sphereOffset =
        sphereDist2 > sphereDist1 ? cap.height / 2.0 : -cap.height / 2.0;

    if (sphereDist <= 0)
    {
        CollisionSphere sphere;
        sphere.body = cap.body;
        sphere.radius = cap.radius;
        sphere.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, sphereOffset, 0));
        sphere.calculateInternals();
        return sphereAndHalfSpace(sphere, plane, data);
    }
    else
    {
        // No need to do cyl check. There's no way for it to just hit that.
        return 0;
    }

    /*ds_math::Vector3 upVec = ds_math::Vector3(cap.getAxis(1));
    ds_math::scalar sphere1 = ds_math::Vector3::Dot(upVec, cap.getAxis(1) *
    cap.height/2.0);
    ds_math::scalar sphere2 = ds_math::Vector3::Dot(upVec, cap.getAxis(1) *
    -cap.height/2.0);*/

    // ds_math::scalar
}

unsigned CollisionDetector::capsuleAndSphere(const CollisionCapsule &cap,
                                             const CollisionSphere &sphere,
                                             CollisionData *data)
{

    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((cap.body) && (cap.body->getMass() == 0)) || !(cap.body)) &&
        (((sphere.body) && (sphere.body->getMass() == 0)) || !(sphere.body)))
        return 0;

    auto capSphereOrigin1 =
        cap.getAxis(3) + cap.getAxis(1) * (cap.height / 2.0);
    auto capSphereOrigin2 =
        cap.getAxis(3) - cap.getAxis(1) * (cap.height / 2.0);
    auto capSphereDist1 =
        ds_math::Vector3::Dot(cap.getAxis(1), capSphereOrigin1);
    auto capSphereDist2 =
        ds_math::Vector3::Dot(cap.getAxis(1), capSphereOrigin2);
    auto sphereDist = ds_math::Vector3::Dot(cap.getAxis(1), sphere.getAxis(3));

    if (sphereDist >= capSphereDist1)
    {
        CollisionSphere ssphere;
        ssphere.body = cap.body;
        ssphere.radius = cap.radius;
        ssphere.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, cap.height / 2.0, 0));
        ssphere.calculateInternals();
        return sphereAndSphere(ssphere, sphere, data);
    }
    else if (sphereDist <= capSphereDist2)
    {
        CollisionSphere ssphere;
        ssphere.body = cap.body;
        ssphere.radius = cap.radius;
        ssphere.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, -cap.height / 2.0, 0));
        ssphere.calculateInternals();
        return sphereAndSphere(ssphere, sphere, data);
    }
    else
    {
        // Make sure we have contacts
        // if (data->contactsLeft <= 0) return 0;

        // Cache the sphere positions
        // (Get the position of the sphere along the capsule's up axis)
        ds_math::Vector3 positionOne =
            sphere.getAxis(3) -
            ds_math::Vector3::Cross(
                cap.getAxis(1),
                ds_math::Vector3::Cross(cap.getAxis(1),
                                        cap.getAxis(3) - sphere.getAxis(3)));
        ds_math::Vector3 positionTwo = sphere.getAxis(3);

        // Find the vector between the objects
        ds_math::Vector3 midline = positionOne - positionTwo;
        ds_math::scalar size = midline.Magnitude();

        // See if it is large enough.
        if (size <= 0.0f || size >= cap.radius + sphere.radius)
        {
            return 0;
        }

        // We manually create the normal, because we have the
        // size to hand.
        ds_math::Vector3 normal = midline * ((1.0) / size);

        Contact *contact = data->contacts;
        contact->contactNormal = normal;
        contact->contactPoint = positionOne + midline * 0.5;
        contact->penetration = (cap.radius + sphere.radius - size);

        contact->setBodyData(
            cap.body ? (cap.body->getMass() == 0 ? nullptr : cap.body)
                     : (nullptr),
            sphere.body ? (sphere.body->getMass() == 0 ? nullptr : sphere.body)
                        : (nullptr),
            data->friction, data->restitution);

        data->addContacts(1);
        return 1;
    }

    return 0;
}

///**
// * Calculates a perpendicular arbitrary axis
// * Credit:
// http://stackoverflow.com/questions/19649452/given-a-single-arbitrary-unit-vector-what-is-the-best-method-to-compute-an-arbi
// * @param v
// * @return
// */
// static ds_math::Vector3 calcArbitraryPerpendicularAxis(const
// ds_math::Vector3& v) {
//	  float length = hypotf( v.x, hypotf(v.y, v.z));
//	  float dir_scalar = (v.x > 0.0) ? length : -length;
//	  float xt = v.x + dir_scalar;
//	  float dot = -v.y / (dir_scalar * xt);
//
//	  return ds_math::Vector3(dot * xt,  1.0f + dot * v.y,  dot * v.z);
//}

unsigned CollisionDetector::capsuleAndBox(const CollisionCapsule &cap,
                                          const CollisionBox &box,
                                          CollisionData *data)
{

    // Make sure we have contacts
    if (data->contactsLeft <= 0)
        return 0;

    if ((((cap.body) && (cap.body->getMass() == 0)) || !(cap.body)) &&
        (((box.body) && (box.body->getMass() == 0)) || !(box.body)))
        return 0;

    auto capSphereOrigin1 =
        cap.getAxis(3) + cap.getAxis(1) * (cap.height / 2.0);
    auto capSphereOrigin2 =
        cap.getAxis(3) - cap.getAxis(1) * (cap.height / 2.0);
    auto capSphereDist1 =
        ds_math::Vector3::Dot(cap.getAxis(1), capSphereOrigin1);
    auto capSphereDist2 =
        ds_math::Vector3::Dot(cap.getAxis(1), capSphereOrigin2);
    auto boxDist = ds_math::Vector3::Dot(cap.getAxis(1), box.getAxis(3));

    if (boxDist >= capSphereDist1)
    {
        CollisionSphere ssphere;
        ssphere.body = cap.body;
        ssphere.radius = cap.radius;
        ssphere.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, cap.height / 2.0, 0));
        ssphere.calculateInternals();
        return boxAndSphere(box, ssphere, data);
    }
    else if (boxDist <= capSphereDist2)
    {
        CollisionSphere ssphere;
        ssphere.body = cap.body;
        ssphere.radius = cap.radius;
        ssphere.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, -cap.height / 2.0, 0));
        ssphere.calculateInternals();
        return boxAndSphere(box, ssphere, data);
    }
    else
    {
        // @bug USing a cube to approximate a cylinder
        CollisionBox sbox;
        sbox.body = cap.body;
        sbox.halfSize =
            ds_math::Vector3(cap.radius, cap.height / 2.0, cap.radius);
        sbox.calculateInternals();
        return boxAndBox(sbox, box, data);
    }

    return 0;
}

unsigned CollisionDetector::capsuleAndCapsule(const CollisionCapsule &cap1,
                                              const CollisionCapsule &cap2,
                                              CollisionData *data)
{


    if ((((cap1.body) && (cap1.body->getMass() == 0)) || !(cap1.body)) &&
        (((cap2.body) && (cap2.body->getMass() == 0)) || !(cap2.body)))
        return 0;

    //@bug VERY UNOPTIMISED (should only need to do one collision check really)
    unsigned totalResult = 0;

    {
        // Make sure we have contacts
        if (data->contactsLeft <= 0)
            return 0;

        CollisionSphere c2s1;
        c2s1.body = cap1.body;
        c2s1.radius = cap1.radius;
        c2s1.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, cap2.height / 2.0, 0));
        c2s1.calculateInternals();

        totalResult += capsuleAndSphere(cap1, c2s1, data);
    }

    {
        // Make sure we have contacts
        if (data->contactsLeft <= 0)
            return 0;

        CollisionSphere c2s2;
        c2s2.body = cap1.body;
        c2s2.radius = cap1.radius;
        c2s2.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, -cap2.height / 2.0, 0));
        c2s2.calculateInternals();

        totalResult += capsuleAndSphere(cap1, c2s2, data);
    }

    {
        // Make sure we have contacts
        if (data->contactsLeft <= 0)
            return 0;

        CollisionSphere c1s1;
        c1s1.body = cap1.body;
        c1s1.radius = cap1.radius;
        c1s1.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, cap1.height / 2.0, 0));
        c1s1.calculateInternals();

        totalResult += capsuleAndSphere(cap2, c1s1, data);
    }

    {
        // Make sure we have contacts
        if (data->contactsLeft <= 0)
            return 0;

        CollisionSphere c1s2;
        c1s2.body = cap1.body;
        c1s2.radius = cap1.radius;
        c1s2.offset = ds_math::Matrix4::CreateTranslationMatrix(
            ds_math::Vector3(0, -cap1.height / 2.0, 0));
        c1s2.calculateInternals();

        totalResult += capsuleAndSphere(cap2, c1s2, data);
    }

    {
        // Make sure we have contacts
        if (data->contactsLeft <= 0)
            return 0;

        //@bug There's probbably a bug in here. Cyl v. Cyl code.

        auto cap1SphereOrigin1 =
            cap1.getAxis(3) + cap1.getAxis(1) * (cap1.height / 2.0);
        auto cap1SphereOrigin2 =
            cap1.getAxis(3) - cap1.getAxis(1) * (cap1.height / 2.0);
        auto cap1SphereDist1 =
            ds_math::Vector3::Dot(cap1.getAxis(1), cap1SphereOrigin1);
        auto cap1SphereDist2 =
            ds_math::Vector3::Dot(cap1.getAxis(1), cap1SphereOrigin2);
        auto cap1MinDist = std::min(cap1SphereDist1, cap1SphereDist2);
        auto cap1MaxDist = std::max(cap1SphereDist1, cap1SphereDist2);

        auto cap2SphereOrigin1 =
            cap2.getAxis(3) + cap2.getAxis(1) * (cap2.height / 2.0);
        auto cap2SphereOrigin2 =
            cap2.getAxis(3) - cap2.getAxis(1) * (cap2.height / 2.0);
        auto cap2SphereDist1 =
            ds_math::Vector3::Dot(cap1.getAxis(1), cap2SphereOrigin1);
        auto cap2SphereDist2 =
            ds_math::Vector3::Dot(cap1.getAxis(1), cap2SphereOrigin2);
        auto cap2MinDist = std::min(cap2SphereDist1, cap2SphereDist2);
        auto cap2MaxDist = std::max(cap2SphereDist1, cap2SphereDist2);

        // If the cyls overlap
        if (!((cap2MaxDist < cap1MinDist) || (cap2MinDist > cap1MaxDist)))
        {
            ds_math::Vector3 positionOne =
                cap2.getAxis(3) -
                ds_math::Vector3::Cross(
                    cap1.getAxis(1),
                    ds_math::Vector3::Cross(cap1.getAxis(1),
                                            cap1.getAxis(3) - cap2.getAxis(3)));
            ds_math::Vector3 positionTwo =
                cap1.getAxis(3) -
                ds_math::Vector3::Cross(
                    cap1.getAxis(1),
                    ds_math::Vector3::Cross(cap1.getAxis(1),
                                            cap2.getAxis(3) - cap1.getAxis(3)));

            // Find the vector between the objects
            ds_math::Vector3 midline = positionOne - positionTwo;
            ds_math::scalar size = midline.Magnitude();

            // See if it is large enough.
            if (size <= 0.0f || size >= cap1.radius + cap2.radius)
            {
                return totalResult;
            }

            // We manually create the normal, because we have the
            // size to hand.
            ds_math::Vector3 normal = midline * ((1.0) / size);

            Contact *contact = data->contacts;
            contact->contactNormal = normal;
            contact->contactPoint = positionOne + midline * 0.5;
            contact->penetration = (cap1.radius + cap2.radius - size);

            contact->setBodyData(
                cap1.body ? (cap1.body->getMass() == 0 ? nullptr : cap1.body)
                          : (nullptr),
                cap2.body ? (cap2.body->getMass() == 0 ? nullptr : cap2.body)
                          : (nullptr),
                data->friction, data->restitution);

            data->addContacts(1);
            totalResult += 1;
        }
    }


    return totalResult;
}
