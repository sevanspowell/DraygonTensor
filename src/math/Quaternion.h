#pragma once

#include <iostream>

#include "Vector3.h"

namespace bb_math
{
class Quaternion
{
public:
    /**
     * Initialize quaternion with given values.
     *
     * @param  x  scalar, first component of quaternion.
     * @param  y  scalar, second component of quaternion.
     * @param  z  scalar, third component of quaternion.
     * @param  w  scalar, fourth component of quaternion.
     */
    Quaternion(scalar x = 0.0f,
               scalar y = 0.0f,
               scalar z = 0.0f,
               scalar w = 1.0f);
    /**
     * Initialize quaternion with an axis and a fourth component.
     *
     * @param  vectorPart  Vector3, axis to initialize quaternion with.
     * @param  scalarPart  scalar, fourth component of quaternion.
     */
    Quaternion(Vector3 vectorPart, scalar scalarPart);
    /**
     * Copy constructor.
     *
     * Create a Quaternion that is a copy of the given Quaternion.
     *
     * @param  other  const Quaternion &, Quaternion to copy.
     */
    Quaternion(const Quaternion &other);

    /**
     * Copy assignment operator.
     *
     * Assign value of other Quaternion to this Quaternion.
     *
     * @param   other  const Quaternion &, Quaternion to copy.
     * @return         const Quaternion &, resulting value of assignment operation
     *  (i.e. other Quaternion).
     */
    const Quaternion &operator=(const Quaternion &other);
    /**
     * Combo scalar multiplication-assignment operator.
     *
     * Multiply all components of the quaternion by factor.
     *
     * @param   factor  scalar, factor to multiply components of quaternion by.
     * @return          const Quaternion &, resulting scaled quaternion.
     */
    const Quaternion &operator*=(scalar factor);
    /**
     * Subscript operator, access components of quaternion.
     *
     * @pre  0 <= index < 4
     *
     * @param   index  unsigned int, index of component to access.
     * @return         scalar, component of quaternion at given index.
     */
    scalar &operator[](unsigned int index);
    /**
     * Const subscript operator, access components of quaternion.
     *
     * @pre  0 <= index < 4
     *
     * @param   index  unsigned int, index of component to access.
     * @return         scalar, component of quaternion at given index.
     */
    const scalar &operator[](unsigned int index) const;
    /**
     * Equivalence operator.
     *
     * Compare two quaternions and return TRUE if they are equal or FALSE
     * otherwise.
     *
     * @param   other  const Quaternion &, Quaternion to compare to this
     *  Quaternion.
     * @return         bool, TRUE if two quaternions are equal, FALSE
     *  otherwise.
     */
    bool operator==(const Quaternion &other) const;
    /**
     * Inequivalence operator.
     *
     * Compare two quaternions and return TRUE if they are not equal or FALSE
     * otherwise.
     *
     * @param   other  const Quaternion &, Quaternion to compare to this
     *  Quaternion.
     * @return         bool, TRUE if two quaternions are not equal, FALSE
     *  otherwise.
     */
    bool operator!=(const Quaternion &other) const;

    /**
     * Get the magnitude of this quaternion.
     *
     * @return  scalar, magnitude of this quaternion.
     */
    scalar Magnitude() const;
    /**
     * Normalize this quaternion.
     *
     * @pre  Magnitude of quaternion is not 0.
     */
    void Normalize();
    /**
     * Invert quaternion.
     */
    void Invert();

    /**
     * Return the magnitude of the quaternion.
     *
     * @param   q  const Quaternion &, quaternion to get magnitude of.
     * @return     scalar, magnitude of quaternion.
     */
    static scalar Magnitude(const Quaternion &q);
    /**
     * Return a normalized version of the given quaternion.
     *
     * @pre  Magnitude of quaternion is not 0.
     *
     * @param   q  const Quaternion &, quaternion to normalize.
     * @return     Quaternion, normalized quaternion.
     */
    static Quaternion Normalize(const Quaternion &q);
    /**
     * Return the quaternion dot product of two given quaternions.
     *
     * @param   q1  const Quaternion &, quaternion argument.
     * @param   q2  const Quaternion &, quaternion argument.
     * @return      scalar, dot product of two given quaternions.
     */
    static scalar Dot(const Quaternion &q1, const Quaternion &q2);
    /**
     * Invert a quaternion.
     *
     * @param   q  const Quaternion &, quaternion to invert.
     * @return     Quaternion, inverted quaternion.
     */
    static Quaternion Invert(const Quaternion &q);

    scalar x, y, z, w;
};

// Binary operators
/**
 * Multiply two quaternions.
 *
 * @param   q1  const Quaternion &, quaternion to multiply.
 * @param   q2  const Quaternion &, quaternion to multiply by.
 * @return      Quaternion, resulting quaternion.
 */
Quaternion operator*(const Quaternion &q1, const Quaternion &q2);
/**
 * Multiply a quaternion by a factor.
 *
 * @param   q       const Quaternion &, quaternion to multiply.
 * @param   factor  scalar, factor to multiply quaternion by.
 * @return          Quaternion, quaternion multiplied by factor.
 */
Quaternion operator*(const Quaternion &q, scalar factor);
/**
 * Multiply a quaternion by a factor.
 *
 * @param   factor  scalar, factor to multiply quaternion by.
 * @param   q       const Quaternion &, quaternion to multiply.
 * @return          Quaternion, quaternion multiplied by factor.
 */
Quaternion operator*(scalar factor, const Quaternion &q);

// Unary operators
/**
 * Invert a quaternion.
 *
 * @param   q  const Quaternion &, quaternion to invert.
 * @return     Quaternion, inverted quaternion.
 */
Quaternion operator-(const Quaternion &q);

/**
 * Output stream operator.
 *
 * Ouput quaternion contents to a stream.
 *
 * @param   out  std::ostream &, output stream to output to.
 * @param   vec  const Quaternion &, quaternion to output.
 * @return       std::ostream &, output stream outputted to.
 */
std::ostream &operator<<(std::ostream &out, const Quaternion &q);
}
