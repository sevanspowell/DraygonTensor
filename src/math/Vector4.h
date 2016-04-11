#pragma once

#include "Precision.h"
#include "Vector3.h"

namespace bb_math
{
class Vector4
{
public:
    /**
     * Construct a Vector4 from the given x, y, z and w values.
     *
     * @param  x  scalar, x-value to give Vector4.
     * @param  y  scalar, y-value to give Vector4.
     * @param  z  scalar, z-value to give Vector4.
     * @param  w  scalar, w-value to give Vector4.
     */
    Vector4(scalar x = 0.0f, scalar y = 0.0f, scalar z = 0.0f, scalar w = 1.0f);
    /**
     * Construct a Vector4 from the given Vector3 and additional w value.
     *
     * @param  vec  const Vector3 &, Vector3 to use for first three values
     *              of a new Vector4.
     * @param  w    scalar, w-value to give Vector4.
     */
    Vector4(const Vector3 &vec, scalar w = 1.0f);
    /**
     * Copy constructor.
     *
     * Create a Vector4 that is a copy of the given Vector4.
     *
     * @param  other  const Vector4 &, Vector4 to copy.
     */
    Vector4(const Vector4 &other);

    /**
     * Copy assignment operator.
     *
     * Assign value of other Vector4 to this Vector4.
     *
     * @param   other  const Vector4 &, Vector4 to copy.
     * @return         const Vector4 &, resulting value of assignment operation
     *  (i.e. other Vector4).
     */
    const Vector4 &operator=(const Vector4 &other);
    /**
     * Indexed member access operator.
     *
     * [0] = x
     * [1] = y
     * [2] = z
     * [3] = w
     *
     * @pre  0 <= index < 4
     *
     * @param  index  unsigned int, index of Vector4 element to access.
     * @return        scalar &, appropriate element.
     */
    scalar &operator[](unsigned int index);
    /**
     * Const indexed member access operator.
     *
     * [0] = x
     * [1] = y
     * [2] = z
     * [3] = w
     *
     * @pre  0 <= index < 4
     *
     * @param   index  unsigned int, index of Vector4 element to access.
     * @return         const scalar &, appropriate element.
     */
    const scalar &operator[](unsigned int index) const;
    /**
     * Combo scalar multiplication-assignment operator.
     *
     * Multiply all components of the vector by factor.
     *
     * @param   factor  scalar, factor to multiply components of vector by.
     * @return          const Vector4 &, resulting scaled vector.
     */
    const Vector4 &operator*=(scalar factor);
    /**
     * Combo addition-assignment operator.
     *
     * Assign value of (other Vector4 + this Vector4) (component-wise addition)
     * to this Vector4.
     *
     * @param   other  const Vector4 &, Vector4 to sum with this Vector4.
     * @return         const Vector4 &, resulting value of operation
     *  (i.e. this + other).
     */
    const Vector4 &operator+=(const Vector4 &other);
    /**
     * Combo subtraction-assignment operator.
     *
     * Assign value of (other Vector4 - this Vector4) (component wise
     * subtraction) to this Vector4.
     *
     * @param   other  const Vector4 &, Vector4 to subtract from this Vector4.
     * @return         const Vector4 &, resulting value of operation
     *  (i.e. this - other).
     */
    const Vector4 &operator-=(const Vector4 &other);
    /**
     * Equivalence operator.
     *
     * Compare two Vector4s and return TRUE if they are equal or FALSE
     * otherwise.
     *
     * @param   other  const Vector4 &, Vector4 to compare to this Vector4.
     * @return         bool, TRUE if two Vector4s are equal, FALSE otherwise.
     */
    bool operator==(const Vector4 &other) const;
    /**
     * Inequivalence operator.
     *
     * Compare two Vector4s and return TRUE if they are equal or FALSE
     * otherwise.
     *
     * @param   other  const Vector4 &, Vector4 to compare to this Vector4.
     * @return         bool, TRUE if two Vector4s are equal, FALSE otherwise.
     */
    bool operator!=(const Vector4 &other) const;

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
     * Return the inverse of the given vector.
     *
     * Change the sign of each component of the vector.
     */
    void Invert();

    /**
     * Find the dot product of the two given vectors.
     *
     * @param   v1  const Vector4 &, first vector argument.
     * @param   v2  const Vector4 &, second vector argument.
     * @return      scalar, dot product of the two given vectors.
     */
    static scalar Dot(const Vector4 &v1, const Vector4 &v2);
    /**
     * Return the magnitude of the given vector,
     *
     * @param   vec  const Vector4 &, vector to find the magnitude of.
     * @return       scalar, magnitude of given vector.
     */
    static scalar Magnitude(const Vector4 &vec);
    /**
     * Return a vector in the same direction as the given vector
     * but with a magnitude of 1.
     *
     * @pre Magnitude of vector is not 0.
     *
     * @param   vec  const Vector4 &, vector to normalize.
     * @return       Vector4, vector in same direction as given vector, but
     *  with a magnitude of 1.
     */
    static Vector4 Normalize(const Vector4 &vec);
    /**
     * Return the inverse of the given vector.
     *
     * Change the sign of each component of the given vector.
     *
     * @param   vec  const Vector4 &, vector to invert.
     * @return       Vector4, inverted vector.
     */
    static Vector4 Invert(const Vector4 &vec);

    /** Unit vector in the X direction */
    static const Vector4 UnitX;
    /** Unit vector in the Y direction */
    static const Vector4 UnitY;
    /** Unit vector in the Z direction */
    static const Vector4 UnitZ;
    /** Unit vector in the W direction */
    static const Vector4 UnitW;

    scalar x, y, z, w;
};

// Binary operators
/**
 * Binary addition operator.
 *
 * Sum two Vector4s.
 *
 * @param   v1  const Vector4 &, Vector4 to sum.
 * @param   v2  const Vector4 &, Vector4 to sum.
 * @return      Vector4, result of v1 + v2.
 */
Vector4 operator+(const Vector4 &v1, const Vector4 &v2);
/**
 * Binary subtraction operator.
 *
 * Subtract one Vector4 from another.
 *
 * @param   v1  const Vector4 &, Vector4 to subtract from.
 * @param   v2  const Vector4 &, Vector4 to subtract with.
 * @return      Vector4, result of v1 - v2.
 */
Vector4 operator-(const Vector4 &v1, const Vector4 &v2);
/**
 * Scale vector by factor.
 *
 * @param   factor  scalar, scaling factor.
 * @param   vec     const Vector4 &, Vector4 to scale.
 * @return          Vector4, scaled vector.
 */
Vector4 operator*(scalar factor, const Vector4 &vec);
/**
 * Scale vector by factor.
 *
 * @param  vec     const Vector4 &, Vector4 to scale.
 * @param  factor  scalar, scaling factor.
 * @return         Vector4, scaled vector.
 */
Vector4 operator*(const Vector4 &vec, scalar factor);
/**
 * Invert vector.
 *
 * Change the sign of each of the vectors components.
 *
 * @param   vec  const Vector4 &, vector to invert.
 * @return       Vector4, inverted vector.
 */
Vector4 operator-(const Vector4 &vec);

/**
 * Output stream operator.
 *
 * Ouput vector contents to a stream.
 *
 * @param   out  std::ostream &, output stream to output to.
 * @param   vec  const Vector4 &, vector to output.
 * @return       std::ostream &, output stream outputted to.
 */
std::ostream &operator<<(std::ostream &out, const Vector4 &vec);
}
