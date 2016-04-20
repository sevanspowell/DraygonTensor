#include <cassert>
#include <cmath>

#include "MathHelper.h"
#include "Matrix4.h"

namespace ds_math
{
Matrix4::Matrix4()
{
    data[0] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    data[1] = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    data[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    data[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4::Matrix4(scalar leading)
{
    data[0] = Vector4(leading, 0.0f, 0.0f, 0.0f);
    data[1] = Vector4(0.0f, leading, 0.0f, 0.0f);
    data[2] = Vector4(0.0f, 0.0f, leading, 0.0f);
    data[3] = Vector4(0.0f, 0.0f, 0.0f, leading);
}

Matrix4::Matrix4(const Matrix4 &other)
{
    data[0] = other[0];
    data[1] = other[1];
    data[2] = other[2];
    data[3] = other[3];
}

Matrix4::Matrix4(const Vector4 &col0,
                 const Vector4 &col1,
                 const Vector4 &col2,
                 const Vector4 &col3)
{
    data[0] = col0;
    data[1] = col1;
    data[2] = col2;
    data[3] = col3;
}

Matrix4::Matrix4(scalar el00,
                 scalar el01,
                 scalar el02,
                 scalar el03,
                 scalar el10,
                 scalar el11,
                 scalar el12,
                 scalar el13,
                 scalar el20,
                 scalar el21,
                 scalar el22,
                 scalar el23,
                 scalar el30,
                 scalar el31,
                 scalar el32,
                 scalar el33)
{
    data[0] = Vector4(el00, el10, el20, el30);
    data[1] = Vector4(el01, el11, el21, el31);
    data[2] = Vector4(el02, el12, el22, el32);
    data[3] = Vector4(el03, el13, el23, el33);
}


const Matrix4 &Matrix4::operator=(const Matrix4 &other)
{
    data[0] = other.data[0];
    data[1] = other.data[1];
    data[2] = other.data[2];
    data[3] = other.data[3];

    return (*this);
}

Vector4 &Matrix4::operator[](unsigned int index)
{
    assert(index < 4 && "Attempt to access index outside Matrix4 bounds.");

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
    case 3:
        return data[3];
        break;
    default:
        return data[0];
        break;
    }
}

const Vector4 &Matrix4::operator[](unsigned int index) const
{
    assert(index < 4 && "Attempt to access index outside Matrix4 bounds.");

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
    case 3:
        return data[3];
        break;
    default:
        return data[0];
        break;
    }
}

const Matrix4 &Matrix4::operator*=(scalar factor)
{
    data[0] *= factor;
    data[1] *= factor;
    data[2] *= factor;
    data[3] *= factor;

    return (*this);
}

const Matrix4 &Matrix4::operator+=(const Matrix4 &other)
{
    data[0] += other[0];
    data[1] += other[1];
    data[2] += other[2];
    data[3] += other[3];

    return (*this);
}

const Matrix4 &Matrix4::operator-=(const Matrix4 &other)
{
    data[0] -= other[0];
    data[1] -= other[1];
    data[2] -= other[2];
    data[3] -= other[3];

    return (*this);
}

bool Matrix4::operator==(const Matrix4 &other) const
{
    return ((data[0] == other[0]) && (data[1] == other[1]) &&
            (data[2] == other[2]) && (data[3] == other[3]));
}

bool Matrix4::operator!=(const Matrix4 &other) const
{
    return (!(*this == other));
}

Matrix4 Matrix4::Transpose(const Matrix4 &mat)
{
    return (Matrix4(mat[0].x, mat[0].y, mat[0].z, mat[0].w, mat[1].x, mat[1].y,
                    mat[1].z, mat[1].w, mat[2].x, mat[2].y, mat[2].z, mat[2].w,
                    mat[3].x, mat[3].y, mat[3].z, mat[3].w));
}

Matrix4 Matrix4::Inverse(const Matrix4 &mat)
{
    // Uses Laplace expansion to find determinant and inverse of matrix
    // Find sub-factors (col, row).
    /* Sub-factor is determinant of 2x2 matrix left when:
     *  -you eliminate row and col of 4x4 co-factor
     *  -you eliminate row and col of 3x3 co-factor
     */
    scalar sub00 = mat[2].z * mat[3].w - mat[3].z * mat[2].w;
    scalar sub01 = mat[1].z * mat[3].w - mat[3].z * mat[1].w;
    scalar sub02 = mat[1].z * mat[2].w - mat[2].z * mat[1].w;
    scalar sub03 = mat[0].z * mat[3].w - mat[3].z * mat[0].w;
    scalar sub04 = mat[0].z * mat[2].w - mat[2].z * mat[0].w;
    scalar sub05 = mat[0].z * mat[1].w - mat[1].z * mat[0].w;
    scalar sub06 = mat[1].y * mat[2].w - mat[2].y * mat[1].w;
    scalar sub07 = mat[0].y * mat[3].w - mat[3].y * mat[0].w;
    scalar sub08 = mat[0].y * mat[1].w - mat[1].y * mat[0].w;
    scalar sub10 = mat[0].y * mat[1].z - mat[1].y * mat[0].z;
    scalar sub11 = mat[0].y * mat[2].z - mat[2].y * mat[0].z;
    scalar sub12 = mat[2].y * mat[3].w - mat[3].y * mat[2].w;
    scalar sub13 = mat[1].y * mat[3].w - mat[3].y * mat[1].w;
    scalar sub14 = mat[1].y * mat[2].w - mat[2].y * mat[1].w;
    scalar sub15 = mat[0].y * mat[2].w - mat[2].y * mat[0].w;
    scalar sub16 = mat[2].y * mat[3].z - mat[3].y * mat[2].z;
    scalar sub17 = mat[1].y * mat[3].z - mat[3].y * mat[1].z;
    scalar sub18 = mat[1].y * mat[2].z - mat[2].y * mat[1].z;
    scalar sub19 = mat[0].y * mat[3].z - mat[3].y * mat[0].z;

    // Find co-factor matrix (col, row)
    scalar co00 = mat[1].y * (sub00)-mat[2].y * (sub01) + mat[3].y * (sub02);
    scalar co10 = -(mat[0].y * (sub00)-mat[2].y * (sub03) + mat[3].y * (sub04));
    scalar co20 = mat[0].y * (sub01)-mat[1].y * (sub03) + mat[3].y * (sub05);
    scalar co30 = -(mat[0].y * (sub02)-mat[1].y * (sub04) + mat[2].y * (sub05));

    scalar co01 = -(mat[1].x * (sub00)-mat[2].x * (sub01) + mat[3].x * (sub02));
    scalar co11 = mat[0].x * (sub00)-mat[2].x * (sub03) + mat[3].x * (sub04);
    scalar co21 = -(mat[0].x * (sub01)-mat[1].x * (sub03) + mat[3].x * (sub05));
    scalar co31 = mat[0].x * (sub02)-mat[1].x * (sub04) + mat[2].x * (sub05);

    scalar co02 = mat[1].x * (sub12)-mat[2].x * (sub13) + mat[3].x * (sub14);
    scalar co12 = -(mat[0].x * (sub12)-mat[2].x * (sub07) + mat[3].x * (sub15));
    scalar co22 = mat[0].x * (sub13)-mat[1].x * (sub07) + mat[3].x * (sub08);
    scalar co32 = -(mat[0].x * (sub06)-mat[1].x * (sub15) + mat[2].x * (sub08));

    scalar co03 = -(mat[1].x * (sub16)-mat[2].x * (sub17) + mat[3].x * (sub18));
    scalar co13 = mat[0].x * (sub16)-mat[2].x * (sub19) + mat[3].x * (sub11);

    scalar co23 = -(mat[0].x * (sub17)-mat[1].x * (sub19) + mat[3].x * (sub10));
    scalar co33 = mat[0].x * (sub18)-mat[1].x * (sub11) + mat[2].x * (sub10);

    // Transpose co-factor matrix to get adjoint matrix (note row-major order of
    // constructor)
    Matrix4 adj = Matrix4(co00, co10, co20, co30, co01, co11, co21, co31, co02,
                          co12, co22, co32, co03, co13, co23, co33);
    adj = Matrix4::Transpose(adj);

    scalar invDet = 1 / (mat[0].x * co00 + mat[1].x * co10 + mat[2].x * co20 +
                         mat[3].x * co30);

    // Multiply adjoint matrix by inverse of determinant to get inverse matrix
    Matrix4 inv = adj * invDet;

    return (inv);
}


Matrix4 Matrix4::CreateOrthographic(scalar width,
                                    scalar height,
                                    scalar nearPlaneDistance,
                                    scalar farPlaneDistance)
{
    scalar xScale = (scalar)2.0f / width;
    scalar yScale = (scalar)2.0f / height;
    scalar zScale = (scalar)1.0f / (farPlaneDistance - nearPlaneDistance);
    scalar pZ = (-nearPlaneDistance) / (farPlaneDistance - nearPlaneDistance);

    Matrix4 projectionMatrix =
        Matrix4::CreateScaleMatrix(xScale, yScale, zScale);
    projectionMatrix[3].z = pZ;

    return projectionMatrix;
}

Matrix4 Matrix4::CreatePerspectiveFieldOfView(scalar fieldOfView,
                                              scalar aspectRatio,
                                              scalar nearPlaneDistance,
                                              scalar farPlaneDistance)
{
    scalar range = tan(fieldOfView * 0.5) * nearPlaneDistance;
    scalar xScale = ((scalar)2.0f * nearPlaneDistance) /
                    ((scalar)2.0f * range * aspectRatio);
    scalar yScale = nearPlaneDistance / range;
    scalar zScale = (-(farPlaneDistance + nearPlaneDistance)) /
                    (farPlaneDistance - nearPlaneDistance);
    scalar pZ = (-((scalar)2.0f * farPlaneDistance * nearPlaneDistance) /
                 (farPlaneDistance - nearPlaneDistance));

    Matrix4 projectionMatrix =
        Matrix4::CreateScaleMatrix(xScale, yScale, zScale);
    projectionMatrix[2].w = -(scalar)1.0f;
    projectionMatrix[3].z = pZ;
    projectionMatrix[3].w = 0.0f;

    return projectionMatrix;
}

Matrix4 Matrix4::CreateTranslationMatrix(const Vector3 &translation)
{
    Matrix4 translationMatrix = Matrix4(1.0f);

    translationMatrix[3] = translation;

    return translationMatrix;
}

Matrix4 Matrix4::CreateTranslationMatrix(scalar x, scalar y, scalar z)
{
    return (Matrix4::CreateTranslationMatrix(Vector3(x, y, z)));
}

Matrix4 Matrix4::CreateScaleMatrix(const Vector3 &scale)
{
    Matrix4 scaleMatrix = Matrix4(1.0f);

    scaleMatrix[0].x = scale.x;
    scaleMatrix[1].y = scale.y;
    scaleMatrix[2].z = scale.z;

    return scaleMatrix;
}

Matrix4 Matrix4::CreateScaleMatrix(float x, float y, float z)
{
    return (Matrix4::CreateScaleMatrix(Vector3(x, y, z)));
}

Matrix4 Matrix4::CreateFromQuaternion(const ds_math::Quaternion &quaternion)
{
    // Mat4 columns
    Vector4 v0 = Vector4(
        1 - 2 * quaternion.y * quaternion.y - 2 * quaternion.z * quaternion.z,
        2 * quaternion.x * quaternion.y + 2 * quaternion.w * quaternion.z,
        2 * quaternion.x * quaternion.z - 2 * quaternion.w * quaternion.y,
        0.0f);
    Vector4 v1 = Vector4(
        2 * quaternion.x * quaternion.y - 2 * quaternion.w * quaternion.z,
        1 - 2 * quaternion.x * quaternion.x - 2 * quaternion.z * quaternion.z,
        2 * quaternion.y * quaternion.z + 2 * quaternion.w * quaternion.x,
        0.0f);
    Vector4 v2 = Vector4(
        2 * quaternion.x * quaternion.z + 2 * quaternion.w * quaternion.y,
        2 * quaternion.y * quaternion.z - 2 * quaternion.w * quaternion.x,
        1 - 2 * quaternion.x * quaternion.x - 2 * quaternion.y * quaternion.y,
        0.0f);
    Vector4 v3 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

    return (Matrix4(v0, v1, v2, v3));
}

Matrix4 operator+(const Matrix4 &m1, const Matrix4 &m2)
{
    return (Matrix4(m1.data[0] + m2.data[0], m1.data[1] + m2.data[1],
                    m1.data[2] + m2.data[2], m1.data[3] + m2.data[3]));
}

Matrix4 operator-(const Matrix4 &m1, const Matrix4 &m2)
{
    return (Matrix4(m1.data[0] - m2.data[0], m1.data[1] - m2.data[1],
                    m1.data[2] - m2.data[2], m1.data[3] - m2.data[3]));
}

Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2)
{
    Vector4 row0(m1[0].x, m1[1].x, m1[2].x, m1[3].x);
    Vector4 row1(m1[0].y, m1[1].y, m1[2].y, m1[3].y);
    Vector4 row2(m1[0].z, m1[1].z, m1[2].z, m1[3].z);
    Vector4 row3(m1[0].w, m1[1].w, m1[2].w, m1[3].w);

    Vector4 resultColumn0(Vector4::Dot(row0, m2[0]), Vector4::Dot(row1, m2[0]),
                          Vector4::Dot(row2, m2[0]), Vector4::Dot(row3, m2[0]));
    Vector4 resultColumn1(Vector4::Dot(row0, m2[1]), Vector4::Dot(row1, m2[1]),
                          Vector4::Dot(row2, m2[1]), Vector4::Dot(row3, m2[1]));
    Vector4 resultColumn2(Vector4::Dot(row0, m2[2]), Vector4::Dot(row1, m2[2]),
                          Vector4::Dot(row2, m2[2]), Vector4::Dot(row3, m2[2]));
    Vector4 resultColumn3(Vector4::Dot(row0, m2[3]), Vector4::Dot(row1, m2[3]),
                          Vector4::Dot(row2, m2[3]), Vector4::Dot(row3, m2[3]));

    return (
        Matrix4(resultColumn0, resultColumn1, resultColumn2, resultColumn3));
}

Vector4 operator*(const Matrix4 &mat, const Vector4 &column)
{
    Vector4 row0(mat[0].x, mat[1].x, mat[2].x, mat[3].x);
    Vector4 row1(mat[0].y, mat[1].y, mat[2].y, mat[3].y);
    Vector4 row2(mat[0].z, mat[1].z, mat[2].z, mat[3].z);
    Vector4 row3(mat[0].w, mat[1].w, mat[2].w, mat[3].w);

    return (Vector4(Vector4::Dot(row0, column), Vector4::Dot(row1, column),
                    Vector4::Dot(row2, column), Vector4::Dot(row3, column)));
}

Vector4 operator*(const Vector4 &row, const Matrix4 &mat)
{
    return (Vector4(Vector4::Dot(row, mat[0]), Vector4::Dot(row, mat[1]),
                    Vector4::Dot(row, mat[2]), Vector4::Dot(row, mat[3])));
}

Matrix4 operator*(const Matrix4 &mat, scalar factor)
{
    Matrix4 scaled = mat;

    scaled *= factor;

    return scaled;
}

Matrix4 operator*(scalar factor, const Matrix4 &mat)
{
    Matrix4 scaled = mat;

    scaled *= factor;

    return scaled;
}

std::ostream &operator<<(std::ostream &out, const Matrix4 &mat)
{
    out << "{{" << mat[0][0] << ", " << mat[1][0] << ", " << mat[2][0] << ", "
        << mat[3][0] << "}, {" << mat[0][1] << ", " << mat[1][1] << ", "
        << mat[2][1] << ", " << mat[3][1] << "}, {" << mat[0][2] << ", "
        << mat[1][2] << ", " << mat[2][2] << ", " << mat[3][2] << "}, {"
        << mat[0][3] << ", " << mat[1][3] << ", " << mat[2][3] << ", "
        << mat[3][3] << "}}";

    return out;
}
}
