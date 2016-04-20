#include <iostream>

#include "gtest/gtest.h"

#include "math/Matrix4.h"

TEST(Matrix4, TestDefaultConstructor)
{
    ds_math::Matrix4 mat = ds_math::Matrix4();

    EXPECT_EQ(ds_math::Vector4(1.0f, 0.0f, 0.0f, 0.0f), mat[0]);
    EXPECT_EQ(ds_math::Vector4(0.0f, 1.0f, 0.0f, 0.0f), mat[1]);
    EXPECT_EQ(ds_math::Vector4(0.0f, 0.0f, 1.0f, 0.0f), mat[2]);
    EXPECT_EQ(ds_math::Vector4(0.0f, 0.0f, 0.0f, 1.0f), mat[3]);
}

TEST(Matrix4, TestLeadingConstructor)
{
    ds_math::scalar a = 3.0f;
    ds_math::Matrix4 mat = ds_math::Matrix4(a);

    EXPECT_EQ(ds_math::Vector4(a, 0.0f, 0.0f, 0.0f), mat[0]);
    EXPECT_EQ(ds_math::Vector4(0.0f, a, 0.0f, 0.0f), mat[1]);
    EXPECT_EQ(ds_math::Vector4(0.0f, 0.0f, a, 0.0f), mat[2]);
    EXPECT_EQ(ds_math::Vector4(0.0f, 0.0f, 0.0f, a), mat[3]);
}

TEST(Matrix4, TestElementConstructor)
{
    ds_math::Matrix4 mat = ds_math::Matrix4(
        23.3f, 10.0f, 0.0f, -2.333f, 10e3f, -10e-4f, -200.0f, 0.1f, 0.3f,
        993.4f, 0.32f, -3.3f, 9073.3f, 2.3f, -3.3f, 4.9f);

    EXPECT_EQ(ds_math::Vector4(23.3f, 10e3f, 0.3f, 9073.3f), mat[0]);
    EXPECT_EQ(ds_math::Vector4(10.0f, -10e-4f, 993.4f, 2.3f), mat[1]);
    EXPECT_EQ(ds_math::Vector4(0.0f, -200.0f, 0.32f, -3.3f), mat[2]);
    EXPECT_EQ(ds_math::Vector4(-2.333f, 0.1f, -3.3f, 4.9f), mat[3]);
}

TEST(Matrix4, TestVectorConstructor)
{
    ds_math::Matrix4 mat =
        ds_math::Matrix4(ds_math::Vector4(23.3f, 10.0f, 0.0f, -2.333f),
                         ds_math::Vector4(10e3f, -10e-4f, -200.0f, 0.1f),
                         ds_math::Vector4(0.3f, 993.4f, 0.32f, -3.3f),
                         ds_math::Vector4(9073.3f, 2.3f, -3.3f, 4.9f));

    EXPECT_EQ(ds_math::Vector4(23.3f, 10.0f, 0.0f, -2.333f), mat[0]);
    EXPECT_EQ(ds_math::Vector4(10e3f, -10e-4f, -200.0f, 0.1f), mat[1]);
    EXPECT_EQ(ds_math::Vector4(0.3f, 993.4f, 0.32f, -3.3f), mat[2]);
    EXPECT_EQ(ds_math::Vector4(9073.3f, 2.3f, -3.3f, 4.9f), mat[3]);
}

TEST(Matrix4, TestEquivalenceOperator)
{
    ds_math::Matrix4 mat1(1.0f);
    ds_math::Matrix4 mat2(1.0f);

    EXPECT_TRUE(mat1 == mat2);

    mat2[2].x = 1.0f;

    EXPECT_FALSE(mat1 == mat2);
}

TEST(Matrix4, TestInequivalenceOperator)
{
    ds_math::Matrix4 mat1(1.0f);
    ds_math::Matrix4 mat2(1.0f);

    EXPECT_FALSE(mat1 != mat2);

    mat2[2].x = 1.0f;

    EXPECT_TRUE(mat1 != mat2);
}

TEST(Matrix4, TestCopyConstructor)
{
    ds_math::Matrix4 mat1 = ds_math::Matrix4(3.0f);
    ds_math::Matrix4 mat2 = ds_math::Matrix4(mat1);

    EXPECT_EQ(mat1, mat2);
}

TEST(Matrix4, TestCopyAssignmentOperator)
{
    ds_math::Matrix4 mat1 = ds_math::Matrix4(3.0f);
    ds_math::Matrix4 mat2 = mat1;

    EXPECT_EQ(mat1, mat2);
}

TEST(Matrix4, TestIndexOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat(col0, col1, col2, col3);

    EXPECT_EQ(col0, mat[0]);
    EXPECT_EQ(col1, mat[1]);
    EXPECT_EQ(col2, mat[2]);
    EXPECT_EQ(col3, mat[3]);
}

TEST(Matrix4, TestConstIndexOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    const ds_math::Matrix4 mat(col0, col1, col2, col3);

    EXPECT_EQ(col0, mat[0]);
    EXPECT_EQ(col1, mat[1]);
    EXPECT_EQ(col2, mat[2]);
    EXPECT_EQ(col3, mat[3]);
}

TEST(Matrix4, TestMultiplicationAssignmentOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat(col0, col1, col2, col3);
    ds_math::scalar factor = 0.24f;
    mat *= factor;

    EXPECT_EQ(col0 * factor, mat[0]);
    EXPECT_EQ(col1 * factor, mat[1]);
    EXPECT_EQ(col2 * factor, mat[2]);
    EXPECT_EQ(col3 * factor, mat[3]);
}

TEST(Matrix4, TestAdditionAssignmentOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat1(col0, col1, col2, col3);
    ds_math::Matrix4 mat2(col0, col1, col2, col3);

    mat1 += mat2;

    EXPECT_EQ(col0 + col0, mat1[0]);
    EXPECT_EQ(col1 + col1, mat1[1]);
    EXPECT_EQ(col2 + col2, mat1[2]);
    EXPECT_EQ(col3 + col3, mat1[3]);
}

TEST(Matrix4, TestSubtractionAssignmentOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat1(col0, col1, col2, col3);
    ds_math::Matrix4 mat2(col0, col1, col2, col3);

    mat1 -= mat2;

    EXPECT_EQ(col0 - col0, mat1[0]);
    EXPECT_EQ(col1 - col1, mat1[1]);
    EXPECT_EQ(col2 - col2, mat1[2]);
    EXPECT_EQ(col3 - col3, mat1[3]);
}

TEST(Matrix4, TestTranspose)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Vector4 transRow0(col0.x, col1.x, col2.x, col3.x);
    ds_math::Vector4 transRow1(col0.y, col1.y, col2.y, col3.y);
    ds_math::Vector4 transRow2(col0.z, col1.z, col2.z, col3.z);
    ds_math::Vector4 transRow3(col0.w, col1.w, col2.w, col3.w);

    ds_math::Matrix4 mat(col0, col1, col2, col3);

    mat = ds_math::Matrix4::Transpose(mat);

    EXPECT_EQ(transRow0, mat[0]);
    EXPECT_EQ(transRow1, mat[1]);
    EXPECT_EQ(transRow2, mat[2]);
    EXPECT_EQ(transRow3, mat[3]);
}

TEST(Matrix4, TestInverse)
{
    ds_math::Matrix4 identity = ds_math::Matrix4();

    EXPECT_EQ(identity, ds_math::Matrix4::Inverse(identity));

    ds_math::Matrix4 mat =
        ds_math::Matrix4(122.0f, 1.0f, 3.0f, 23.0f, 32.0f, 3.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f);

    ds_math::Matrix4 result = ds_math::Matrix4(
        3 / 334.0f, -1 / 334.0f, -9 / 1336.0f, -69 / 1670.0f, -16 / 167.0f,
        61 / 167.0f, 12 / 167.0f, 368 / 835.0f, 0.0f, 0.0f, 1 / 4.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1 / 5.0f);

    ds_math::Matrix4 inv = ds_math::Matrix4::Inverse(mat);

    EXPECT_EQ(result, inv);
}

TEST(Matrix4, TestCreateTranslationMatrix)
{
    ds_math::Vector3 translation(1.0f, -2.3f, 0.3f);
    ds_math::Matrix4 mat =
        ds_math::Matrix4::CreateTranslationMatrix(translation);
    ds_math::Matrix4 expectedResult(1.0f);
    expectedResult[3] = ds_math::Vector4(translation, 1.0f);

    EXPECT_EQ(expectedResult, mat);

    mat = ds_math::Matrix4::CreateTranslationMatrix(
        translation.x, translation.y, translation.z);

    EXPECT_EQ(expectedResult, mat);
}

TEST(Matrix4, TestCreateScaleMatrix)
{
    ds_math::Vector3 scale(3.4f, 0.2f, -2.33f);
    ds_math::Matrix4 mat = ds_math::Matrix4::CreateScaleMatrix(scale);

    ds_math::Matrix4 expectedResult(1.0f);
    expectedResult[0].x = scale.x;
    expectedResult[1].y = scale.y;
    expectedResult[2].z = scale.z;

    EXPECT_EQ(expectedResult, mat);

    mat = ds_math::Matrix4::CreateScaleMatrix(scale.x, scale.y, scale.z);

    EXPECT_EQ(expectedResult, mat);
}

TEST(Matrix4, TestBinaryAdditionOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat1(col0, col1, col2, col3);
    ds_math::Matrix4 mat2(col0, col1, col2, col3);

    ds_math::Matrix4 mat3 = mat1 + mat2;

    EXPECT_EQ(col0 + col0, mat3[0]);
    EXPECT_EQ(col1 + col1, mat3[1]);
    EXPECT_EQ(col2 + col2, mat3[2]);
    EXPECT_EQ(col3 + col3, mat3[3]);
}

TEST(Matrix4, TestBinarySubtractionOperator)
{
    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat1(col0, col1, col2, col3);
    ds_math::Matrix4 mat2(col0, col1, col2, col3);

    ds_math::Matrix4 mat3 = mat1 - mat2;

    EXPECT_EQ(col0 - col0, mat3[0]);
    EXPECT_EQ(col1 - col1, mat3[1]);
    EXPECT_EQ(col2 - col2, mat3[2]);
    EXPECT_EQ(col3 - col3, mat3[3]);
}

TEST(Matrix4, TestMatrixMultiplication)
{
    ds_math::Matrix4 identity;
    ds_math::Matrix4 mat1 =
        ds_math::Matrix4::CreateScaleMatrix(3.0f, 3.0f, 3.0f);

    EXPECT_EQ(mat1, identity * mat1);

    ds_math::Matrix4 mat2 =
        ds_math::Matrix4::CreateScaleMatrix(2.0f, 2.0f, 2.0f);

    EXPECT_EQ(ds_math::Matrix4::CreateScaleMatrix(6.0f, 6.0f, 6.0f),
              mat1 * mat2);
}

TEST(Matrix4, TestMatrixColumnMultiplication)
{
    ds_math::Vector3 translate = ds_math::Vector3(10.0f, 3.04f, -4.3f);
    ds_math::Matrix4 mat = ds_math::Matrix4::CreateTranslationMatrix(translate);

    ds_math::Vector4 point = ds_math::Vector4(1.0f, 3.0f, 10.0f);

    ds_math::Vector4 result = mat * point;

    EXPECT_EQ(point.x + translate.x, result.x);
    EXPECT_EQ(point.y + translate.y, result.y);
    EXPECT_EQ(point.z + translate.z, result.z);
}

TEST(Matrix4, TestMatrixRowMultiplication)
{
    ds_math::scalar scale = 3.0f;

    ds_math::Matrix4 mat =
        ds_math::Matrix4::CreateScaleMatrix(scale, scale, scale);

    ds_math::Vector4 point = ds_math::Vector4(1.0f, 3.0f, 10.0f);

    ds_math::Vector4 result = point * mat;

    EXPECT_EQ(point.x * scale, result.x);
    EXPECT_EQ(point.y * scale, result.y);
    EXPECT_EQ(point.z * scale, result.z);
}

TEST(Matrix4, TestMatrixFactorMultiplication)
{
    ds_math::scalar factor = 3.44f;

    ds_math::Vector4 col0(0.3f, 10e3f, 0.98e5f, -2.3f);
    ds_math::Vector4 col1(-0.3f, -10e3f, 0.928e5f, 2.3f);
    ds_math::Vector4 col2(0.33f, 8e3f, -0.98e5f, -24.33f);
    ds_math::Vector4 col3(3.0f, -2e3f, 0.9338e5f, -3.3f);

    ds_math::Matrix4 mat1(col0, col1, col2, col3);

    mat1 = mat1 * factor;

    EXPECT_EQ(col0 * factor, mat1[0]);
    EXPECT_EQ(col1 * factor, mat1[1]);
    EXPECT_EQ(col2 * factor, mat1[2]);
    EXPECT_EQ(col3 * factor, mat1[3]);

    mat1 = ds_math::Matrix4(col0, col1, col2, col3);

    mat1 = factor * mat1;

    EXPECT_EQ(col0 * factor, mat1[0]);
    EXPECT_EQ(col1 * factor, mat1[1]);
    EXPECT_EQ(col2 * factor, mat1[2]);
    EXPECT_EQ(col3 * factor, mat1[3]);
}

TEST(Matrix4, TestOutputStreamOperator)
{
    ds_math::Matrix4 mat;

    std::stringstream stream, resultStream;

    resultStream << "{{" << mat[0][0] << ", " << mat[1][0] << ", " << mat[2][0]
                 << ", " << mat[3][0] << "}, {" << mat[0][1] << ", "
                 << mat[1][1] << ", " << mat[2][1] << ", " << mat[3][1]
                 << "}, {" << mat[0][2] << ", " << mat[1][2] << ", "
                 << mat[2][2] << ", " << mat[3][2] << "}, {" << mat[0][3]
                 << ", " << mat[1][3] << ", " << mat[2][3] << ", " << mat[3][3]
                 << "}}";
    stream << mat;

    EXPECT_EQ(resultStream.str(), stream.str());
}
