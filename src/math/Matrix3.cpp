#include <cassert>

#include "Matrix3.h"

namespace ds_math
{
Matrix3::Matrix3()
{
    data[0] = Vector3(1.0f, 0.0f, 0.0f);
    data[1] = Vector3(0.0f, 1.0f, 0.0f);
    data[2] = Vector3(0.0f, 0.0f, 1.0f);
}
Matrix3::Matrix3(scalar leading)
{
    data[0] = Vector3(leading, 0.0f, 0.0f);
    data[1] = Vector3(0.0f, leading, 0.0f);
    data[2] = Vector3(0.0f, 0.0f, leading);
}
Matrix3::Matrix3(const Matrix3 &other)
{
    data[0] = other[0];
    data[1] = other[1];
    data[2] = other[2];
}
Matrix3::Matrix3(const Vector3 &col0, const Vector3 &col1, const Vector3 &col2)
{
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
}
Matrix3::Matrix3(scalar el00,
                 scalar el01,
                 scalar el02,
                 scalar el10,
                 scalar el11,
                 scalar el12,
                 scalar el20,
                 scalar el21,
                 scalar el22)
{
    data[0] = Vector3(el00, el10, el20);
    data[1] = Vector3(el01, el11, el21);
    data[2] = Vector3(el02, el12, el22);
}

const Matrix3 &Matrix3::operator=(const Matrix3 &other)
{
    data[0] = other.data[0];
    data[1] = other.data[1];
    data[2] = other.data[2];

    return (*this);
}
Vector3 &Matrix3::operator[](unsigned int index)
{
    assert(index < 3 && "Attempt to access index outside Matrix3 bounds.");

    switch (index)
    {
    case 0:
        return data[0];
        break;
    case 1:
        return data[1];
        break;
    case 2:
        return data[2];
        break;
    default:
        return data[0];
        break;
    }
}
const Vector3 &Matrix3::operator[](unsigned int index) const
{
    assert(index < 3 && "Attempt to access index outside Matrix3 bounds.");

    switch (index)
    {
    case 0:
        return data[0];
        break;
    case 1:
        return data[1];
        break;
    case 2:
        return data[2];
        break;
    default:
        return data[0];
        break;
    }
}
const Matrix3 &Matrix3::operator*=(scalar factor)
{
    data[0] *= factor;
    data[1] *= factor;
    data[2] *= factor;

    return (*this);
}
const Matrix3 &Matrix3::operator+=(const Matrix3 &other)
{
    data[0] += other[0];
    data[1] += other[1];
    data[2] += other[2];

    return (*this);
}
const Matrix3 &Matrix3::operator-=(const Matrix3 &other)
{
    data[0] -= other[0];
    data[1] -= other[1];
    data[2] -= other[2];

    return (*this);
}
bool Matrix3::operator==(const Matrix3 &other) const
{
    return ((data[0] == other[0]) && (data[1] == other[1]) &&
            (data[2] == other[2]));
}
bool Matrix3::operator!=(const Matrix3 &other) const
{
    return (!(*this == other));
}

Matrix3 Matrix3::Transpose(const Matrix3 &mat)
{
    return (Matrix3(mat[0].x, mat[0].y, mat[0].z, mat[1].x, mat[1].y, mat[1].z,
                    mat[2].x, mat[2].y, mat[2].z));
}
Matrix3 Matrix3::Inverse(const Matrix3 &mat)
{
    // Credit to Ogre engine
    // Taken from:
    // https://bitbucket.org/sinbad/ogre/src/3cbd67467fab3fef44d1b32bc42ccf4fb1ccfdd0/OgreMain/src/OgreMatrix3.cpp?at=default&fileviewer=file-view-default
    Matrix3 inv;

    inv[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
    inv[0][1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
    inv[0][2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
    inv[1][0] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
    inv[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
    inv[1][2] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
    inv[2][0] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
    inv[2][1] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
    inv[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    scalar det =
        mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0];

    // If determinant not 0 or close to it
    if (fabs(det) > ds_math::FLOAT_ACCURACY)
    {
        scalar invDet = 1.0f / det;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                inv[iRow][iCol] *= invDet;
        }
    }

    return inv;
}

Vector3 Matrix3::Transform(const Matrix3 &mat, const Vector3 &vec)
{
    return mat * vec;
}


Vector3 Matrix3::TransformInverse(const Matrix3 &mat, const Vector3 &vec)
{
    return Matrix3::Inverse(mat) * vec;
}

// Binary operators
Matrix3 operator+(const Matrix3 &m1, const Matrix3 &m2)
{
    return (Matrix3(m1.data[0] + m2.data[0], m1.data[1] + m2.data[1],
                    m1.data[2] + m2.data[2]));
}
Matrix3 operator-(const Matrix3 &m1, const Matrix3 &m2)
{
    return (Matrix3(m1.data[0] - m2.data[0], m1.data[1] - m2.data[1],
                    m1.data[2] - m2.data[2]));
}
Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2)
{
    Vector3 resultColumn0(m1 * m2[0]);
    Vector3 resultColumn1(m1 * m2[1]);
    Vector3 resultColumn2(m1 * m2[2]);

    return (Matrix3(resultColumn0, resultColumn1, resultColumn2));
}
Vector3 operator*(const Matrix3 &mat, const Vector3 &column)
{
    Vector3 row0(mat[0].x, mat[1].x, mat[2].x);
    Vector3 row1(mat[0].y, mat[1].y, mat[2].y);
    Vector3 row2(mat[0].z, mat[1].z, mat[2].z);

    return (Vector3(Vector3::Dot(row0, column), Vector3::Dot(row1, column),
                    Vector3::Dot(row2, column)));
}
Vector3 operator*(const Vector3 &row, const Matrix3 &mat)
{
    return (Vector3(Vector3::Dot(row, mat[0]), Vector3::Dot(row, mat[1]),
                    Vector3::Dot(row, mat[2])));
}
Matrix3 operator*(const Matrix3 &mat, scalar factor)
{
    Matrix3 scaled = mat;

    scaled *= factor;

    return scaled;
}
Matrix3 operator*(scalar factor, const Matrix3 &mat)
{
    Matrix3 scaled = mat;

    scaled *= factor;

    return scaled;
}

std::ostream &operator<<(std::ostream &out, const Matrix3 &mat)
{
    out << "{{" << mat[0][0] << ", " << mat[1][0] << ", " << mat[2][0] << "}, {"
        << mat[0][1] << ", " << mat[1][1] << ", " << mat[2][1] << "}, {"
        << mat[0][2] << ", " << mat[1][2] << ", " << mat[2][2] << "}}";

    return out;
}
}
