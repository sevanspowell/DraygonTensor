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
#pragma once

#include <math/Matrix4.h>
#include <math/Precision.h>
#include <math/Vector3.h>
#include <engine/system/physics/RigidBody.h>
#include <engine/system/physics/Contacts.h>
#include <cassert>

/*

Must compelte Contacts.cpp Contacts.h before
the problems in CollisionFine.cpp can be resolved
as a lot of the data types are defined in contacts

//some problems

    getAxisVector not ds_math::Matrix4
    replace () with []
    getTransform not ds_phys::RigidBody

    normalise not ds_math::Vector3
    transform not ds_math::Matrix4

*/

namespace ds_phys
{

// forward declaration of CollisionPrimitive friends
class IntersectionTests;
class CollisionDetector;

class CollisionPrimitive
{

public:
    /**
    * This class exists to help the collision detector
    * and intersection routines, so they should have
    * access to its data.
    */
    friend class IntersectionTests;
    friend class CollisionDetector;

    /**
    * The rigid body that is represented by this primitive.
    */
    RigidBody *body;

    /**
    * The offset of this primitive from the given rigid body.
    */
    ds_math::Matrix4 offset;

    CollisionPrimitive();
    virtual ~CollisionPrimitive() {}

    /**
    * Calculates the internals for the primitive.
    */
    void calculateInternals();

    /**
    * This is a convenience function to allow access to the
    * axis vectors in the transform for this primitive.
    */
    ds_math::Vector3 getAxis(unsigned index) const
    {
        return transform[index];
    }

    /**
    * Returns the resultant transform of the primitive, calculated from
    * the combined offset of the primitive and the transform
    * (orientation + position) of the rigid body to which it is
    * attached.
    */
    const ds_math::Matrix4 &getTransform() const
    {
        return transform;
    }

protected:
    /**
    * The resultant transform of the primitive. This is
    * calculated by combining the offset of the primitive
    * with the transform of the rigid body.
    */
    ds_math::Matrix4 transform;


}; // end class CollisionPrimitive

class CollisionBox : public CollisionPrimitive
{
public:
    /**
    * Holds the half-sizes of the box along each of its local axes.
    */
    ds_math::Vector3 halfSize;
}; // end class CollisionBox

class CollisionSphere : public CollisionPrimitive
{
public:
    /**
     * Radius of the sphere.
     */
    ds_math::scalar radius;
};

class CollisionPlane : public CollisionPrimitive
{
public:
    /**
     * Plane normal.
     */
    ds_math::Vector3 direction;

    /**
     * Distance of the plane from the origin.
     */
    ds_math::scalar offset;
};

class CollisionCapsule : public CollisionPrimitive
{
	public:
		ds_math::scalar height;
		ds_math::scalar radius;
};

class IntersectionTests
{
public:
    static bool sphereAndHalfSpace(const CollisionSphere &sphere,
                                   const CollisionPlane &plane);

    /*static bool sphereAndSphere(const CollisionSphere &one,
                                const CollisionSphere &two); //Unused */

    static bool boxAndBox(const CollisionBox &one, const CollisionBox &two);

    static bool capsuleAndHalfSpace(const CollisionCapsule& cap, const CollisionPlane &plane);

    /**
    * Does an intersection test on an arbitrarily aligned box and a
    * half-space.
    *
    * The box is given as a transform matrix, including
    * position, and a vector of half-sizes for the extend of the
    * box along each local axis.
    *
    * The half-space is given as a direction (i.e. unit) vector and the
    * offset of the limiting plane from the origin, along the given
    * direction.
    */
    static bool boxAndHalfSpace(const CollisionBox &box,
                                const CollisionPlane &plane);


}; // end IntersectionTests


/**
    * A helper structure that contains information for the detector to use
    * in building its contact data.
    */
struct CollisionData
{
    /**
    * Holds the base of the collision data: the first contact
    * in the array. This is used so that the contact pointer (below)
    * can be incremented each time a contact is detected, while
    * this pointer points to the first contact found.
    */
    Contact *contactArray;

    /** Holds the contact array to write into. */
    Contact *contacts;

    /** Holds the maximum number of contacts the array can take. */
    int contactsLeft;

    /** Holds the number of contacts found so far. */
    unsigned contactCount;

    /** Holds the friction value to write into any collisions. */
    ds_math::scalar friction;

    /** Holds the restitution value to write into any collisions. */
    ds_math::scalar restitution;

    /**
    * Holds the collision tolerance, even uncolliding objects this
    * close should have collisions generated.
    */
    ds_math::scalar tolerance;

    /**
    * Checks if there are more contacts available in the contact
    * data.
    */
    bool hasMoreContacts()
    {
        return contactsLeft > 0;
    }

    /**
        * Resets the data so that it has no used contacts recorded.
        */
    void reset(unsigned maxContacts)
    {
        contactsLeft = maxContacts;
        contactCount = 0;
        contacts = contactArray;
    }

    /**
    * Notifies the data that the given number of contacts have
    * been added.
    */
    void addContacts(unsigned count)
    {
        // Reduce the number of contacts remaining, add number used
        contactsLeft -= count;
        assert(contactsLeft >= 0 && "Contact array too full.");
        contactCount += count;

        // Move the array forward
        contacts += count;
    }
};

/**
* A wrapper class that holds the fine grained collision detection
* routines.
*
* Each of the functions has the same format: it takes the details
* of two objects, and a pointer to a contact array to fill. It
* returns the number of contacts it wrote into the array.
*/
class CollisionDetector
{
public:
    static unsigned sphereAndHalfSpace(const CollisionSphere &sphere,
                                       const CollisionPlane &plane,
                                       CollisionData *data);

    static unsigned sphereAndSphere(const CollisionSphere &one,
                                    const CollisionSphere &two,
                                    CollisionData *data);

    /**
        * Does a collision test on a collision box and a plane representing
        * a half-space (i.e. the normal of the plane
        * points out of the half-space).
        */
    static unsigned boxAndHalfSpace(const CollisionBox &box,
                                    const CollisionPlane &plane,
                                    CollisionData *data);

    static unsigned boxAndBox(const CollisionBox &one,
                              const CollisionBox &two,
                              CollisionData *data);

    static unsigned boxAndPoint(const CollisionBox &box,
                                const ds_math::Vector3 &point,
                                CollisionData *data);

    static unsigned boxAndSphere(const CollisionBox &box,
                                 const CollisionSphere &sphere,
                                 CollisionData *data);

    static unsigned capsuleAndHalfSpace(const CollisionCapsule &cap,
            const CollisionPlane &plane,
            CollisionData *data);

    static unsigned capsuleAndSphere(const CollisionCapsule &cap,
            const CollisionSphere &sphere,
            CollisionData *data);


    static unsigned capsuleAndBox(const CollisionCapsule &cap,
            const CollisionBox &box,
            CollisionData *data);
}; // end CollisionDetector

} // end namespace
