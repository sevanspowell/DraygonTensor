#include "gtest/gtest.h"

#include "math/Vector3.h"

TEST(Vector3, TestDefaultConstructor)
{
    ds_math::Vector3 vec = ds_math::Vector3();

    EXPECT_EQ(0.0f, vec.x);
    EXPECT_EQ(0.0f, vec.y);
    EXPECT_EQ(0.0f, vec.z);
}

TEST(Vector3, TestConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    EXPECT_EQ(x, vec.x);
    EXPECT_EQ(y, vec.y);
    EXPECT_EQ(z, vec.z);
}

TEST(Vector3, TestCopyConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec1 = ds_math::Vector3(x, y, z);
    ds_math::Vector3 vec2 = ds_math::Vector3(vec1);

    EXPECT_EQ(vec1.x, vec2.x);
    EXPECT_EQ(vec1.y, vec2.y);
    EXPECT_EQ(vec1.z, vec2.z);
}

TEST(Vector3, TestIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
}

TEST(Vector3, TestConstIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    const ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
}

TEST(Vector3, TestMultiplicationAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float factor = -3.99f;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);
    vec *= factor;

    EXPECT_EQ(x * factor, vec.x);
    EXPECT_EQ(y * factor, vec.y);
    EXPECT_EQ(z * factor, vec.z);
}

TEST(Vector3, TestAdditionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec1 = ds_math::Vector3(x, y, z);
    ds_math::Vector3 vec2 = ds_math::Vector3(x, y, z);
    vec1 += vec2;

    EXPECT_EQ(x * 2, vec1.x);
    EXPECT_EQ(y * 2, vec1.y);
    EXPECT_EQ(z * 2, vec1.z);
}

TEST(Vector3, TestSubtractionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec1 = ds_math::Vector3(x, y, z);
    ds_math::Vector3 vec2 = ds_math::Vector3(x, y, z);
    vec1 -= vec2;

    EXPECT_EQ(0.0f, vec1.x);
    EXPECT_EQ(0.0f, vec1.y);
    EXPECT_EQ(0.0f, vec1.z);
}

TEST(Vector3, TestEquivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec1 = ds_math::Vector3(x, y, z);
    ds_math::Vector3 vec2 = ds_math::Vector3(x, y, z);

    EXPECT_TRUE(vec1 == vec2);

    vec2.z = 0.3f;
    EXPECT_FALSE(vec1 == vec2);
}

TEST(Vector3, TestInequivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    ds_math::Vector3 vec1 = ds_math::Vector3(x, y, z);
    ds_math::Vector3 vec2 = ds_math::Vector3(x, y, z);

    EXPECT_FALSE(vec1 != vec2);

    vec2.z = 0.3f;
    EXPECT_TRUE(vec1 != vec2);
}

TEST(Vector3, TestMagnitude)
{
    float x = 2;
    float y = 2;
    float z = -99.0f;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    EXPECT_TRUE(fabs(vec.Magnitude() - sqrt((x * x) + (y * y) + (z * z))) <=
                ds_math::FLOAT_ACCURACY);
    EXPECT_TRUE(fabs(ds_math::Vector3::Magnitude(vec) -
                     sqrt((x * x) + (y * y) + (z * z))) <=
                ds_math::FLOAT_ACCURACY);
}

TEST(Vector3, TestNormalize)
{
    float x = 1.0f;
    float y = 0.0f;
    float z = 0.0f;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    vec.Normalize();
    EXPECT_EQ(ds_math::Vector3(x, y, z), vec);

    x = 993.0f;
    y = 0.2f;
    z = -3.321f;

    vec = ds_math::Vector3(x, y, z);
    float mag = vec.Magnitude();
    vec.Normalize();

    EXPECT_EQ(ds_math::Vector3(x / mag, y / mag, z / mag), vec);

    vec = ds_math::Vector3(x, y, z);
    ds_math::Vector3 normalized = ds_math::Vector3::Normalize(vec);
    vec.Normalize();

    EXPECT_EQ(vec, normalized);
}

TEST(Vector3, TestInvert)
{
    float x = -13.0f;
    float y = 2.0f;
    float z = 0.0f;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);
    vec.Invert();

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);

    vec = ds_math::Vector3(x, y, z);
    vec = ds_math::Vector3::Invert(vec);

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);

    vec = ds_math::Vector3(x, y, z);
    vec = -vec;

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);
}

TEST(Vector3, TestDotProduct)
{
    ds_math::Vector3 vec1 = ds_math::Vector3(3.0f, 1.0f, -23.03f);
    ds_math::Vector3 vec2 = ds_math::Vector3(-3.4f, 100.0f, -3.897f);
    float expectedResult = 179.54791f;

    EXPECT_EQ(expectedResult, ds_math::Vector3::Dot(vec1, vec2));
}

TEST(Vector3, TestCrossProduct)
{
    ds_math::Vector3 vec1 = ds_math::Vector3(3.0f, 1.0f, -23.03f);
    ds_math::Vector3 vec2 = ds_math::Vector3(-3.4f, 100.0f, -3.897f);
    ds_math::Vector3 cross = ds_math::Vector3::Cross(vec1, vec2);

    ds_math::Vector3 expectedResult =
        ds_math::Vector3((vec1.y * vec2.z) - (vec1.z * vec2.y),
                         (vec1.z * vec2.x) - (vec1.x * vec2.z),
                         (vec1.x * vec2.y) - (vec1.y * vec2.x));

    EXPECT_TRUE(expectedResult == cross);
}

TEST(Vector3, TestAdditionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;

    ds_math::Vector3 vec1 = ds_math::Vector3(x1, y1, z1);
    ds_math::Vector3 vec2 = ds_math::Vector3(x2, y2, z2);
    ds_math::Vector3 result = vec1 + vec2;

    EXPECT_EQ(x1 + x2, result.x);
    EXPECT_EQ(y1 + y2, result.y);
    EXPECT_EQ(z1 + z2, result.z);
}

TEST(Vector3, TestSubtractionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;

    ds_math::Vector3 vec1 = ds_math::Vector3(x1, y1, z1);
    ds_math::Vector3 vec2 = ds_math::Vector3(x2, y2, z2);
    ds_math::Vector3 result = vec1 - vec2;

    EXPECT_EQ(x1 - x2, result.x);
    EXPECT_EQ(y1 - y2, result.y);
    EXPECT_EQ(z1 - z2, result.z);
}

TEST(Vector3, TestScaleOperator)
{
    float x = 3.0f;
    float y = -10e-4f;
    float z = 10e9f;
    float scale = -9.4f;

    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    ds_math::Vector3 result = vec * scale;

    EXPECT_EQ(x * scale, result.x);
    EXPECT_EQ(y * scale, result.y);
    EXPECT_EQ(z * scale, result.z);

    result = scale * vec;

    EXPECT_EQ(x * scale, result.x);
    EXPECT_EQ(y * scale, result.y);
    EXPECT_EQ(z * scale, result.z);
}

TEST(Vector3, TestOutputStreamOperator)
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
    std::stringstream stream, resultStream;
    ds_math::Vector3 vec = ds_math::Vector3(x, y, z);

    resultStream << "{" << x << ", " << y << ", " << z << "}";
    stream << vec;

    EXPECT_EQ(resultStream.str(), stream.str());
}

TEST(Vector3, TestUnitX)
{
    EXPECT_EQ(ds_math::Vector3(1.0f, 0.0f, 0.0f), ds_math::Vector3::UnitX);
}

TEST(Vector3, TestUnitY)
{
    EXPECT_EQ(ds_math::Vector3(0.0f, 1.0f, 0.0f), ds_math::Vector3::UnitY);
}

TEST(Vector3, TestUnitZ)
{
    EXPECT_EQ(ds_math::Vector3(0.0f, 0.0f, 1.0f), ds_math::Vector3::UnitZ);
}
