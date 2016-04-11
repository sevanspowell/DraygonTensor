#include <cassert>
#include <cmath>

#include "Vector3.h"

namespace bb_math
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
