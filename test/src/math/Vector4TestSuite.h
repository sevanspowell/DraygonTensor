#include "gtest/gtest.h"

#include "math/Vector4.h"

TEST(Vector4, TestDefaultConstructor)
{
    bb_math::Vector4 vec = bb_math::Vector4();

    EXPECT_EQ(0.0f, vec.x);
    EXPECT_EQ(0.0f, vec.y);
    EXPECT_EQ(0.0f, vec.z);
    EXPECT_EQ(1.0f, vec.w);
}

TEST(Vector4, TestConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    EXPECT_EQ(x, vec.x);
    EXPECT_EQ(y, vec.y);
    EXPECT_EQ(z, vec.z);
    EXPECT_EQ(w, vec.w);
}

TEST(Vector4, TestCopyConstructor)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 2e-2f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 vec2 = bb_math::Vector4(vec1);

    EXPECT_EQ(vec1.x, vec2.x);
    EXPECT_EQ(vec1.y, vec2.y);
    EXPECT_EQ(vec1.z, vec2.z);
    EXPECT_EQ(vec1.w, vec2.w);
}

TEST(Vector4, TestIndexOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 2e-2f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
    EXPECT_EQ(w, vec[3]);
}

TEST(Vector4, TestConstIndexOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 2e-2f;

    const bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
    EXPECT_EQ(w, vec[3]);
}

TEST(Vector4, TestMultiplicationAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 83.3e-2f;
    float factor = -3.99f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);
    vec *= factor;

    EXPECT_EQ(x * factor, vec.x);
    EXPECT_EQ(y * factor, vec.y);
    EXPECT_EQ(z * factor, vec.z);
    EXPECT_EQ(w * factor, vec.w);
}

TEST(Vector4, TestAdditionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 8.4e-8f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 vec2 = bb_math::Vector4(x, y, z, w);
    vec1 += vec2;

    EXPECT_EQ(x * 2, vec1.x);
    EXPECT_EQ(y * 2, vec1.y);
    EXPECT_EQ(z * 2, vec1.z);
    EXPECT_EQ(w * 2, vec1.w);
}

TEST(Vector4, TestSubtractionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = 8.4e-8f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 vec2 = bb_math::Vector4(x, y, z, w);
    vec1 -= vec2;

    EXPECT_EQ(0.0f, vec1.x);
    EXPECT_EQ(0.0f, vec1.y);
    EXPECT_EQ(0.0f, vec1.z);
    EXPECT_EQ(0.0f, vec1.w);
}

TEST(Vector4, TestEquivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = -10e-8f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 vec2 = bb_math::Vector4(x, y, z, w);

    EXPECT_TRUE(vec1 == vec2);

    vec2.z = 0.3f;
    EXPECT_FALSE(vec1 == vec2);
}

TEST(Vector4, TestInequivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6f;
    float z = 10e-8f;
    float w = -10e-8f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 vec2 = bb_math::Vector4(x, y, z, w);

    EXPECT_FALSE(vec1 != vec2);

    vec2.z = 0.3f;
    EXPECT_TRUE(vec1 != vec2);
}

TEST(Vector4, TestMagnitude)
{
    float x = 2;
    float y = 2;
    float z = -99.0f;
    float w = 0.3f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    EXPECT_TRUE(
        fabs(vec.Magnitude() - sqrt((x * x) + (y * y) + (z * z) + (w * w))) <=
        bb_math::FLOAT_ACCURACY);
    EXPECT_TRUE(fabs(bb_math::Vector4::Magnitude(vec) -
                     sqrt((x * x) + (y * y) + (z * z) + (w * w))) <=
                bb_math::FLOAT_ACCURACY);
}

TEST(Vector4, TestNormalize)
{
    float x = 1.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    vec.Normalize();
    EXPECT_EQ(bb_math::Vector4(x, y, z, w), vec);

    x = 993.0f;
    y = 0.2f;
    z = -3.321f;
    w = 38.8f;

    vec = bb_math::Vector4(x, y, z, w);
    float mag = vec.Magnitude();
    vec.Normalize();

    EXPECT_EQ(bb_math::Vector4(x / mag, y / mag, z / mag, w / mag), vec);

    vec = bb_math::Vector4(x, y, z, w);
    bb_math::Vector4 normalized = bb_math::Vector4::Normalize(vec);
    vec.Normalize();

    EXPECT_EQ(vec, normalized);
}

TEST(Vector4, TestInvert)
{
    float x = -13.0f;
    float y = 2.0f;
    float z = 0.0f;
    float w = -10.9f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);
    vec.Invert();

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);
    EXPECT_EQ(-w, vec.w);

    vec = bb_math::Vector4(x, y, z, w);
    vec = bb_math::Vector4::Invert(vec);

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);
    EXPECT_EQ(-w, vec.w);

    vec = bb_math::Vector4(x, y, z, w);
    vec = -vec;

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);
    EXPECT_EQ(-w, vec.w);
}

TEST(Vector4, TestDotProduct)
{
    bb_math::Vector4 vec1 = bb_math::Vector4(3.0f, 1.0f, -23.03f, 1.0f);
    bb_math::Vector4 vec2 = bb_math::Vector4(-3.4f, 100.0f, -3.897f, 0.5f);
    float expectedResult = 180.04791f;

    EXPECT_EQ(expectedResult, bb_math::Vector4::Dot(vec1, vec2));
}

TEST(Vector4, TestAdditionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float w1 = -10e2f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;
    float w2 = 10e9f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x1, y1, z1, w1);
    bb_math::Vector4 vec2 = bb_math::Vector4(x2, y2, z2, w2);
    bb_math::Vector4 result = vec1 + vec2;

    EXPECT_EQ(x1 + x2, result.x);
    EXPECT_EQ(y1 + y2, result.y);
    EXPECT_EQ(z1 + z2, result.z);
    EXPECT_EQ(w1 + w2, result.w);
}

TEST(Vector4, TestSubtractionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float w1 = -10e2f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;
    float w2 = 10e9f;

    bb_math::Vector4 vec1 = bb_math::Vector4(x1, y1, z1, w1);
    bb_math::Vector4 vec2 = bb_math::Vector4(x2, y2, z2, w2);
    bb_math::Vector4 result = vec1 - vec2;

    EXPECT_EQ(x1 - x2, result.x);
    EXPECT_EQ(y1 - y2, result.y);
    EXPECT_EQ(z1 - z2, result.z);
    EXPECT_EQ(w1 - w2, result.w);
}

TEST(Vector4, TestScaleOperator)
{
    float x = 3.0f;
    float y = -10e-4f;
    float z = 10e9f;
    float w = -0.11e1f;
    float scale = -9.4f;

    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    bb_math::Vector4 result = vec * scale;

    EXPECT_EQ(x * scale, result.x);
    EXPECT_EQ(y * scale, result.y);
    EXPECT_EQ(z * scale, result.z);
    EXPECT_EQ(w * scale, result.w);

    result = scale * vec;

    EXPECT_EQ(x * scale, result.x);
    EXPECT_EQ(y * scale, result.y);
    EXPECT_EQ(z * scale, result.z);
    EXPECT_EQ(w * scale, result.w);
}

TEST(Vector4, TestOutputStreamOperator)
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
    float w = 0.0f;
    std::stringstream stream, resultStream;
    bb_math::Vector4 vec = bb_math::Vector4(x, y, z, w);

    resultStream << "{" << x << ", " << y << ", " << z << ", " << w << "}";
    stream << vec;

    EXPECT_EQ(resultStream.str(), stream.str());
}

TEST(Vector4, TestUnitX)
{
    EXPECT_EQ(bb_math::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
              bb_math::Vector4::UnitX);
}

TEST(Vector4, TestUnitY)
{
    EXPECT_EQ(bb_math::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
              bb_math::Vector4::UnitY);
}

TEST(Vector4, TestUnitZ)
{
    EXPECT_EQ(bb_math::Vector4(0.0f, 0.0f, 1.0f, 0.0f),
              bb_math::Vector4::UnitZ);
}

TEST(Vector4, TestUnitW)
{
    EXPECT_EQ(bb_math::Vector4(0.0f, 0.0f, 0.0f, 1.0f),
              bb_math::Vector4::UnitW);
}
