#include <cassert>
#include <cmath>

#include "MathHelper.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace ds_math
{
Vector3::Vector3(scalar x, scalar y, scalar z) : x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z)
{
}

const Vector3 &Vector3::operator=(const Vector3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return (*this);
}

scalar &Vector3::operator[](unsigned int index)
{
    assert(index < 3 && "Attempt to access index outside Vector3 bounds.");

    switch (index)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    default:
        return x;
        break;
    }
}

const scalar &Vector3::operator[](unsigned int index) const
{
    assert(index < 3 && "Attempt to access index outside Vector3 bounds.");

    switch (index)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    default:
        return x;
        break;
    }
}

const Vector3 &Vector3::operator*=(scalar factor)
{
    x *= factor;
    y *= factor;
    z *= factor;

    return (*this);
}

const Vector3 &Vector3::operator+=(const Vector3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return (*this);
}

const Vector3 &Vector3::operator-=(const Vector3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return (*this);
}

bool Vector3::operator==(const Vector3 &other) const
{
    return (fabs(x - other.x) < FLOAT_ACCURACY &&
            fabs(y - other.y) < FLOAT_ACCURACY &&
            fabs(z - other.z) < FLOAT_ACCURACY);
}

bool Vector3::operator!=(const Vector3 &other) const
{
    return (!(*this == other));
}

scalar Vector3::Magnitude() const
{
    return Vector3::Magnitude(*this);
}

void Vector3::Normalize()
{
    *this = Vector3::Normalize(*this);
}

void Vector3::Invert()
{
    *this = Vector3::Invert(*this);
}

scalar Vector3::Dot(const Vector3 &v1, const Vector3 &v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

scalar Vector3::Magnitude(const Vector3 &vec)
{
    return sqrt(Vector3::Dot(vec, vec));
}

Vector3 Vector3::Cross(const Vector3 &v1, const Vector3 &v2)
{
    return (Vector3((v1.y * v2.z) - (v1.z * v2.y),
                    (v1.z * v2.x) - (v1.x * v2.z),
                    (v1.x * v2.y) - (v1.y * v2.x)));
}

Vector3 Vector3::Normalize(const Vector3 &vec)
{
    float mag = Vector3::Magnitude(vec);

    assert(mag != 0 && "Attempted to normalize a vector with zero magnitude.");

    return (vec * (((scalar)1.0f) / mag));
}

Vector3 Vector3::Invert(const Vector3 &vec)
{
    return (vec * -1);
}

Vector3 Vector3::Transform(const Vector3 &vec, const Matrix4 &matrix)
{
    Vector4 v = Vector4(vec.x, vec.y, vec.z, 1.0);

    // Transform vector
    v = matrix * v;

    return (Vector3(v.x, v.y, v.z));
}

Quaternion Vector3::GetRotationFromTo(const Vector3 &vec1, const Vector3 &vec2)
{
    Quaternion q;

    /*
     * From:
     * https://bitbucket.org/sinbad/ogre/src/9db75e3ba05c/OgreMain/include/OgreVector3.h?fileviewer=file-view-default#cl-651
     */
    // Normalize two provided vectors
    Vector3 v1 = Vector3::Normalize(vec1);
    Vector3 v2 = Vector3::Normalize(vec2);

    scalar dot = Vector3::Dot(v1, v2);

    // If vectors are parallel
    if (dot >= 1.0f)
    {
        return q;
    }
    // If vector very close to -1 (vectors have an angle of almost 180 degrees
    // between them)
    if (dot < (1e-6f - 1.0f))
    {
        // Generate an axis
        Vector3 axis = Vector3::Cross(Vector3::UnitX, v1);
        // If colinear axis
        if (Vector3::Magnitude(axis) == 0.0f)
        {
            // Pick another axis
            axis = Vector3::Cross(Vector3::UnitY, v1);
        }
        axis.Normalize();

        q = Quaternion::CreateFromAxisAngle(axis, MathHelper::PI);
    }
    else
    {
        scalar s = sqrt((1 + dot) * 2);
        scalar inverse = 1 / s;

        Vector3 c = Vector3::Cross(v1, v2);

        q.x = c.x * inverse;
        q.y = c.y * inverse;
        q.z = c.z * inverse;
        q.w = s * 0.5f;
        q.Normalize();
    }

    return q;
}

const Vector3 Vector3::UnitX = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::UnitY = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::UnitZ = Vector3(0.0f, 0.0f, 1.0f);

Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
    return (Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
    return (Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

Vector3 operator*(scalar factor, const Vector3 &vec)
{
    Vector3 scaled = vec;

    scaled *= factor;

    return scaled;
}

Vector3 operator*(const Vector3 &vec, scalar factor)
{
    Vector3 scaled = vec;

    scaled *= factor;

    return scaled;
}

Vector3 operator-(const Vector3 &vec)
{
    return Vector3::Invert(vec);
}

std::ostream &operator<<(std::ostream &out, const Vector3 &vec)
{
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";

    return out;
}
}
