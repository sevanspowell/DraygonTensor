#pragma once

#include <iostream>

#include "Precision.h"

namespace ds_math
{
class Matrix4;
class Quaternion;
class Vector4;
/**
 *  Vector-3 class.
 */
class Vector3
{
public:
    /**
     * Construct a Vector3 from the given x, y and z values.
     *
     * @param  x  scalar, x-value to give Vector3.
     * @param  y  scalar, y-value to give Vector3.
     * @param  z  scalar, z-value to give Vector3.
     */
    Vector3(scalar x = 0.0f, scalar y = 0.0f, scalar z = 0.0f);
    /**
     * Construct a Vector3 from another Vector3 (copy constructor).
     *
     * @param  other  const Vector3 &, Vector3 to copy.
     */
    Vector3(const Vector3 &other);
    /**
     * Construct a Vector3 from a Vector4.
     *
     * Simply constructs a Vector3 from the x, y, z components of the given
     * Vector4.
     *
     * @param  other  const Vector4 &, Vector4 to 'convert' to Vector3.
     */
    Vector3(const Vector4 &other);

    /**
     * Copy assignment operator.
     *
     * Assign value of another Vector3 to this Vector3.
     *
     * @param       other   const Vector3 &, Vector3 to copy.
     * @return              const Vector3 &, resulting value of assignment
     *  operation (i.e. other Vector3).
     */
    const Vector3 &operator=(const Vector3 &other);
    /**
     * Indexed member access operator.
     *
     * [0] = x
     * [1] = y
     * [2] = z
     *
     * @pre  0 <= index < 3.
     *
     * @param   index  unsigned int, index of Vector3 element to access.
     * @return         scalar &, appropriate element.
     */
    scalar &operator[](unsigned int index);
    /**
     * Const indexed member access operator.
     *
     * [0] = x
     * [1] = y
     * [2] = z
     *
     * @pre  0 <= index < 3.
     *
     * @param  index  unsigned int, index of Vector3 element to access.
     * @return        const scalar &, appropriate element.
     */
    const scalar &operator[](unsigned int index) const;
    /**
    * Scalar multiplication operator.
    *
    * Multiply all components of the vector by factor.
    *
    * @param   factor  scalar, factor to multiply components of vector by.
    * @return          const Vector3 &, resulting scaled vector.
    */
    const Vector3 &operator*=(scalar factor);
    /**
     * Combo addition-assignment operator.
     *
     * Assign value of sum(other Vector3, this Vector3) to this Vector3.
     *
     * @param   other  const Vector3 &, Vector3 to sum with this Vector3.
     * @return         const Vector3 &, resulting value of operation
     *  (i.e. this + other).
     */
    const Vector3 &operator+=(const Vector3 &other);
    /**
     * Combo subtraction-assignment operator.
     *
     * Assign value of (other Vector3 - this Vector3) to this Vector3.
     *
     * @param   other  const Vector3 &, Vector3 to subtract from this Vector3.
     * @return         const Vector3 &, resulting value of operation
     *  (i.e. this - other).
     */
    const Vector3 &operator-=(const Vector3 &other);
    /**
     * Equivalence operator.
     *
     * Compare two Vector3s and return TRUE if they are equal or FALSE
     * otherwise.
     *
     * @param   other  const Vector3 &, Vector3 to compare to this Vector3.
     * @return         bool, TRUE if two Vector3s are equal, FALSE otherwise.
     */
    bool operator==(const Vector3 &other) const;
    /**
     * Inequivalence operator.
     *
     * Compare two Vector3s and return TRUE if they are not equal or FALSE
     * otherwise.
     *
     * @param   other  const Vector3 &, Vector3 to compare to this Vector3.
     * @return         bool, TRUE if two Vector3s are equal, FALSE otherwise.
     */
    bool operator!=(const Vector3 &other) const;

    /**
     * Get the magnitude (length) of this vector.
     *
     * @return  scalar, magnitude (length) of this vector.
     */
    scalar Magnitude() const;
    /**
     * Normalize this vector, causing it's magnitude to be 1.
     *
     * @pre Magnitude of vector is not 0.
     */
    void Normalize();
    /**
     * Invert this given vector.
     *
     * Change the sign of each component of the vector.
     */
    void Invert();
    /**
     * Set all the components of this vector to 0.
     */
    void Clear();

    /**
     * Find the dot product of the two given vectors.
     *
     * @param   v1  const Vector3 &, first vector argument.
     * @param   v2  const Vector3 &, second vector argument.
     * @return      scalar, dot product of the two given vectors.
     */
    static scalar Dot(const Vector3 &v1, const Vector3 &v2);
    /**
     * Return the magnitude of the given vector,
     *
     * @param   vec  const Vector3 &, vector to find the magnitude of.
     * @return       scalar, magnitude of given vector.
     */
    static scalar Magnitude(const Vector3 &vec);
    /**
     * Find the cross product of the two given vectors.
     *
     * @param   v1  const Vector3 &, first vector argument.
     * @param   v2  const Vector3 &, second vector argument.
     * @return      Vector3, cross product of two vectors.
     */
    static Vector3 Cross(const Vector3 &v1, const Vector3 &v2);
    /**
     * Return a vector in the same direction as the given vector but with a
     * magnitude of 1.
     *
     * @pre Magnitude of vector is not 0.
     *
     * @param  vec  const Vector3 &, vector to normalize.
     * @return      Vector3, vector in same direction as given vector, but with
     * a magnitude of 1.
     */
    static Vector3 Normalize(const Vector3 &vec);
    /**
     * Return the inverse of the given vector.
     *
     * Change the sign of each component of the given vector.
     *
     * @param   vec  const Vector3 &, vector to invert.
     * @return       Vector3, inverted vector.
     */
    static Vector3 Invert(const Vector3 &vec);
    /**
     * Get a quaternion representing the rotation from the first provided vector
     * to the second.
     *
     * @param   vec1  const Vector3 &, rotation from this vector.
     * @param   vec2  const Vector3 &, rotation to this vector.
     * @return        Quaternion, quaternion representing rotation from the
     * first provided vector to the second.
     */
    static Quaternion GetRotationFromTo(const Vector3 &vec1,
                                        const Vector3 &vec2);

    /** Unit vector in the X direction. */
    static const Vector3 UnitX;
    /** Unit vector in the Y direction. */
    static const Vector3 UnitY;
    /** Unit vector in the Z direction. */
    static const Vector3 UnitZ;

    scalar x, y, z;
};

// Binary operators
/**
 * Binary addition operator.
 *
 * Sum two Vector3s.
 *
 * @param   v1  const Vector3 &, Vector3 to sum.
 * @param   v2  const Vector3 &, Vector3 to sum.
 * @return      Vector3, result of v1 + v2.
 */
Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
/**
 * Binary subtraction operator.
 *
 * Subtract one Vector3 from another.
 *
 * @param   v1  const Vector3 &, Vector3 to subtract from.
 * @param   v2  const Vector3 &, Vector3 to subtract with.
 * @return      Vector3, result of v1 - v2.
 */
Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
/**
 * Scale vector by factor.
 *
 * @param   factor  scalar, scaling factor.
 * @param   vec     const Vector3 &, Vector3 to scale.
 * @return          Vector3, scaled vector.
 */
Vector3 operator*(scalar factor, const Vector3 &vec);
/**
 * Scale vector by factor.
 *
 * @param   vec     const Vector3 &, Vector3 to scale.
 * @param   factor  scalar, scaling factor.
 * @return          Vector3, scaled vector.
 */
Vector3 operator*(const Vector3 &vec, scalar factor);
/**
 * Invert vector.
 *
 * Change the sign of each of the vectors components.
 *
 * @param   vec  const Vector3 &, vector to invert.
 * @return       Vector3, inverted vector.
 */
Vector3 operator-(const Vector3 &vec);
/**
 * Multiply the components of two vectors.
 *
 * @param   vec1  const Vector3 &, vector 1.
 * @param   vec2  const Vector3 &, vector 2.
 * @return        Vector3, multiplied vector result.
 */
Vector3 operator*(const Vector3 &vec1, const Vector3 &vec2);

/**
 * Output stream operator.
 *
 * Ouput vector contents to a stream.
 *
 * @param   out  std::ostream &, output stream to output to.
 * @param   vec  const Vector3 &, vector to output.
 * @return       std::ostream &, output stream outputted to.
 */
std::ostream &operator<<(std::ostream &out, const Vector3 &vec);
}
