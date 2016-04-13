#include <cmath>
#include <cassert>

#include "Vector4.h"

namespace ds_math
{
Vector4::Vector4(scalar x, scalar y, scalar z, scalar w)
    : x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(const Vector3 &vec, scalar w)
    : x(vec.x), y(vec.y), z(vec.z), w(w)
{
}

Vector4::Vector4(const Vector4 &other)
    : x(other.x), y(other.y), z(other.z), w(other.w)
{
}

const Vector4 &Vector4::operator=(const Vector4 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;

    return (*this);
}

scalar &Vector4::operator[](unsigned int index)
{
    assert(index < 4 && "Attempt to access index outside Vector4 bounds.");

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
    case 3:
        return w;
        break;
    default:
        return x;
        break;
    }
}

const scalar &Vector4::operator[](unsigned int index) const
{
    assert(index < 4 && "Attempt to access index outside Vector4 bounds.");

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
    case 3:
        return w;
        break;
    default:
        return x;
        break;
    }
}

const Vector4 &Vector4::operator*=(scalar factor)
{
    x *= factor;
    y *= factor;
    z *= factor;
    w *= factor;

    return (*this);
}

const Vector4 &Vector4::operator+=(const Vector4 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return (*this);
}

const Vector4 &Vector4::operator-=(const Vector4 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return (*this);
}

bool Vector4::operator==(const Vector4 &other) const
{
    return (fabs(x - other.x) < FLOAT_ACCURACY &&
            fabs(y - other.y) < FLOAT_ACCURACY &&
            fabs(z - other.z) < FLOAT_ACCURACY &&
            fabs(w - other.w) < FLOAT_ACCURACY);
}

bool Vector4::operator!=(const Vector4 &other) const
{
    return (!(*this == other));
}

scalar Vector4::Magnitude() const
{
    return Vector4::Magnitude(*this);
}

void Vector4::Normalize()
{
    *this = Vector4::Normalize(*this);
}

void Vector4::Invert()
{
    *this = Vector4::Invert(*this);
}

scalar Vector4::Dot(const Vector4 &v1, const Vector4 &v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
}

scalar Vector4::Magnitude(const Vector4 &vec)
{
    return sqrt(Vector4::Dot(vec, vec));
}

Vector4 Vector4::Normalize(const Vector4 &vec)
{
    scalar mag = Vector4::Magnitude(vec);

    assert(mag != 0 && "Attempted to normalize a vector with zero magnitude.");

    return (vec * (((scalar)1.0f) / mag));
}

Vector4 Vector4::Invert(const Vector4 &vec)
{
    return (vec * -1);
}

const Vector4 Vector4::UnitX = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

const Vector4 Vector4::UnitY = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

const Vector4 Vector4::UnitZ = Vector4(0.0f, 0.0f, 1.0f, 0.0f);

const Vector4 Vector4::UnitW = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

Vector4 operator+(const Vector4 &v1, const Vector4 &v2)
{
    return (Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w));
}

Vector4 operator-(const Vector4 &v1, const Vector4 &v2)
{
    return (Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w));
}

Vector4 operator*(scalar factor, const Vector4 &vec)
{
    Vector4 scaled = vec;

    scaled *= factor;

    return scaled;
}

Vector4 operator*(const Vector4 &vec, scalar factor)
{
    Vector4 scaled = vec;

    scaled *= factor;

    return scaled;
}

Vector4 operator-(const Vector4 &vec)
{
    return Vector4::Invert(vec);
}

std::ostream &operator<<(std::ostream &out, const Vector4 &vec)
{
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
        << "}";

    return out;
}
}
