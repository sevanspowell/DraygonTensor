#include <iostream>

#include "gtest/gtest.h"

#include "math/Matrix3.h"

TEST(Matrix3, TestDefaultConstructor)
{
    ds_math::Matrix3 mat = ds_math::Matrix3();

    EXPECT_EQ(ds_math::Vector3(1.0f, 0.0f, 0.0f), mat[0]);
    EXPECT_EQ(ds_math::Vector3(0.0f, 1.0f, 0.0f), mat[1]);
    EXPECT_EQ(ds_math::Vector3(0.0f, 0.0f, 1.0f), mat[2]);
}

TEST(Matrix3, TestLeadingConstructor)
{
    ds_math::scalar a = 3.0f;
    ds_math::Matrix3 mat = ds_math::Matrix3(a);

    EXPECT_EQ(ds_math::Vector3(a, 0.0f, 0.0f), mat[0]);
    EXPECT_EQ(ds_math::Vector3(0.0f, a, 0.0f), mat[1]);
    EXPECT_EQ(ds_math::Vector3(0.0f, 0.0f, a), mat[2]);
}

TEST(Matrix3, TestElementConstructor)
{
    ds_math::Matrix3 mat = ds_math::Matrix3(23.3f, 10.0f, 0.0f, 10e3f, -10e-4f,
                                            -200.0f, 0.3f, 993.4f, 0.32f);

    EXPECT_EQ(ds_math::Vector3(23.3f, 10e3f, 0.3f), mat[0]);
    EXPECT_EQ(ds_math::Vector3(10.0f, -10e-4f, 993.4f), mat[1]);
    EXPECT_EQ(ds_math::Vector3(0.0f, -200.0f, 0.32f), mat[2]);
}

TEST(Matrix3, TestVectorConstructor)
{
    ds_math::Matrix3 mat =
        ds_math::Matrix3(ds_math::Vector3(23.3f, 10.0f, 0.0f),
                         ds_math::Vector3(10e3f, -10e-4f, -200.0f),
                         ds_math::Vector3(0.3f, 993.4f, 0.32f));

    EXPECT_EQ(ds_math::Vector3(23.3f, 10.0f, 0.0f), mat[0]);
    EXPECT_EQ(ds_math::Vector3(10e3f, -10e-4f, -200.0f), mat[1]);
    EXPECT_EQ(ds_math::Vector3(0.3f, 993.4f, 0.32f), mat[2]);
}

TEST(Matrix3, TestEquivalenceOperator)
{
    ds_math::Matrix3 mat1(1.0f);
    ds_math::Matrix3 mat2(1.0f);

    EXPECT_TRUE(mat1 == mat2);

    mat2[2].x = 1.0f;

    EXPECT_FALSE(mat1 == mat2);
}

TEST(Matrix3, TestInequivalenceOperator)
{
    ds_math::Matrix3 mat1(1.0f);
    ds_math::Matrix3 mat2(1.0f);

    EXPECT_FALSE(mat1 != mat2);

    mat2[2].x = 1.0f;

    EXPECT_TRUE(mat1 != mat2);
}

TEST(Matrix3, TestCopyConstructor)
{
    ds_math::Matrix3 mat1 = ds_math::Matrix3(3.0f);
    ds_math::Matrix3 mat2 = ds_math::Matrix3(mat1);

    EXPECT_EQ(mat1, mat2);
}

TEST(Matrix3, TestCopyAssignmentOperator)
{
    ds_math::Matrix3 mat1 = ds_math::Matrix3(3.0f);
    ds_math::Matrix3 mat2 = mat1;

    EXPECT_EQ(mat1, mat2);
}

TEST(Matrix3, TestIndexOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat(col0, col1, col2);

    EXPECT_EQ(col0, mat[0]);
    EXPECT_EQ(col1, mat[1]);
    EXPECT_EQ(col2, mat[2]);
}

TEST(Matrix3, TestConstIndexOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    const ds_math::Matrix3 mat(col0, col1, col2);

    EXPECT_EQ(col0, mat[0]);
    EXPECT_EQ(col1, mat[1]);
    EXPECT_EQ(col2, mat[2]);
}

TEST(Matrix3, TestMultiplicationAssignmentOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat(col0, col1, col2);
    ds_math::scalar factor = 0.24f;
    mat *= factor;

    EXPECT_EQ(col0 * factor, mat[0]);
    EXPECT_EQ(col1 * factor, mat[1]);
    EXPECT_EQ(col2 * factor, mat[2]);
}

TEST(Matrix3, TestAdditionAssignmentOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat1(col0, col1, col2);
    ds_math::Matrix3 mat2(col0, col1, col2);

    mat1 += mat2;

    EXPECT_EQ(col0 + col0, mat1[0]);
    EXPECT_EQ(col1 + col1, mat1[1]);
    EXPECT_EQ(col2 + col2, mat1[2]);
}

TEST(Matrix3, TestTranspose)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Vector3 transRow0(col0.x, col1.x, col2.x);
    ds_math::Vector3 transRow1(col0.y, col1.y, col2.y);
    ds_math::Vector3 transRow2(col0.z, col1.z, col2.z);

    ds_math::Matrix3 mat(col0, col1, col2);

    mat = ds_math::Matrix3::Transpose(mat);

    EXPECT_EQ(transRow0, mat[0]);
    EXPECT_EQ(transRow1, mat[1]);
    EXPECT_EQ(transRow2, mat[2]);
}

TEST(Matrix3, TestTransformVector3)
{
    ds_math::scalar a = 1.0f;
    ds_math::scalar b = 0.2f;
    ds_math::scalar c = 0.3f;

    ds_math::Vector3 v(a, b, c);

    v = ds_math::Matrix3::Transform(ds_math::Matrix3(3.0f), v);

    EXPECT_EQ(a * 3.0f, v.x);
    EXPECT_EQ(b * 3.0f, v.y);
    EXPECT_EQ(c * 3.0f, v.z);
}

TEST(Matrix3, TestInverseTransformVector3)
{
    ds_math::scalar a = 1.0f;
    ds_math::scalar b = 0.2f;
    ds_math::scalar c = 0.3f;

    ds_math::Vector3 v(a, b, c);

    v = ds_math::Matrix3::TransformInverse(ds_math::Matrix3(3.0f), v);

    EXPECT_EQ(ds_math::Vector3(a / 3.0f, b / 3.0f, c / 3.0f), v);
}

TEST(Matrix3, TestInverse)
{
    ds_math::Matrix3 identity = ds_math::Matrix3();

    EXPECT_EQ(identity, ds_math::Matrix3::Inverse(identity));

    ds_math::Matrix3 mat = ds_math::Matrix3(122.0f, 1.0f, 3.0f, 32.0f, 3.0f,
                                            0.0f, 0.0f, 0.0f, 4.0f);

    ds_math::Matrix3 result =
        ds_math::Matrix3(3 / 334.0f, -1 / 334.0f, -9 / 1336.0f, -16 / 167.0f,
                         61 / 167.0f, 12 / 167.0f, 0.0f, 0.0f, 1 / 4.0f);

    ds_math::Matrix3 inv = ds_math::Matrix3::Inverse(mat);

    EXPECT_EQ(result, inv);
}

TEST(Matrix3, TestBinaryAdditionOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat1(col0, col1, col2);
    ds_math::Matrix3 mat2(col0, col1, col2);

    ds_math::Matrix3 mat3 = mat1 + mat2;

    EXPECT_EQ(col0 + col0, mat3[0]);
    EXPECT_EQ(col1 + col1, mat3[1]);
    EXPECT_EQ(col2 + col2, mat3[2]);
}

TEST(Matrix3, TestBinarySubtractionOperator)
{
    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat1(col0, col1, col2);
    ds_math::Matrix3 mat2(col0, col1, col2);

    ds_math::Matrix3 mat3 = mat1 - mat2;

    EXPECT_EQ(col0 - col0, mat3[0]);
    EXPECT_EQ(col1 - col1, mat3[1]);
    EXPECT_EQ(col2 - col2, mat3[2]);
}

TEST(Matrix3, TestMatrixMultiplication)
{
    const float a = 3.0f;
    const float b = 2.0f;
    ds_math::Matrix3 identity;
    ds_math::Matrix3 mat1 = ds_math::Matrix3(a);

    EXPECT_EQ(mat1, identity * mat1);

    ds_math::Matrix3 mat2 = ds_math::Matrix3(b);

    EXPECT_EQ(ds_math::Matrix3(a * b), mat1 * mat2);
}

TEST(Matrix3, TestMatrixColumnMultiplication)
{
    const float a = 3.0f;
    ds_math::Matrix3 mat = ds_math::Matrix3(a);

    ds_math::Vector3 point = ds_math::Vector3(1.0f, 3.0f, 10.0f);

    ds_math::Vector3 result = mat * point;

    EXPECT_EQ(point.x * a, result.x);
    EXPECT_EQ(point.y * a, result.y);
    EXPECT_EQ(point.z * a, result.z);
}

TEST(Matrix3, TestMatrixRowMultiplication)
{
    const float a = 3.0f;
    ds_math::Matrix3 mat = ds_math::Matrix3(a);

    ds_math::Vector3 point = ds_math::Vector3(1.0f, 3.0f, 10.0f);

    ds_math::Vector3 result = point * mat;

    EXPECT_EQ(point.x * a, result.x);
    EXPECT_EQ(point.y * a, result.y);
    EXPECT_EQ(point.z * a, result.z);
}

TEST(Matrix3, TestMatrixFactorMultiplication)
{
    ds_math::scalar factor = 3.44f;

    ds_math::Vector3 col0(0.3f, 10e3f, 0.98e5f);
    ds_math::Vector3 col1(-0.3f, -10e3f, 0.928e5f);
    ds_math::Vector3 col2(0.33f, 8e3f, -0.98e5f);

    ds_math::Matrix3 mat1(col0, col1, col2);

    mat1 = mat1 * factor;

    EXPECT_EQ(col0 * factor, mat1[0]);
    EXPECT_EQ(col1 * factor, mat1[1]);
    EXPECT_EQ(col2 * factor, mat1[2]);

    mat1 = ds_math::Matrix3(col0, col1, col2);

    mat1 = factor * mat1;

    EXPECT_EQ(col0 * factor, mat1[0]);
    EXPECT_EQ(col1 * factor, mat1[1]);
    EXPECT_EQ(col2 * factor, mat1[2]);
}

TEST(Matrix3, TestOutputStreamOperator)
{
    ds_math::Matrix3 mat;

    std::stringstream stream, resultStream;

    resultStream << "{{" << mat[0][0] << ", " << mat[1][0] << ", " << mat[2][0]
                 << "}, {" << mat[0][1] << ", " << mat[1][1] << ", "
                 << mat[2][1] << "}, {" << mat[0][2] << ", " << mat[1][2]
                 << ", " << mat[2][2] << "}}";
    stream << mat;

    EXPECT_EQ(resultStream.str(), stream.str());
}
