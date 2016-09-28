#include "gtest/gtest.h"

#include "math/Quaternion.h"

TEST(Quaternion, TestDefaultConstructor)
{
    ds_math::Quaternion q = ds_math::Quaternion();

    EXPECT_EQ(0.0f, q.x);
    EXPECT_EQ(0.0f, q.y);
    EXPECT_EQ(0.0f, q.z);
    EXPECT_EQ(1.0f, q.w);
}

TEST(Quaternion, TestConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);

    EXPECT_EQ(x, q.x);
    EXPECT_EQ(y, q.y);
    EXPECT_EQ(z, q.z);
    EXPECT_EQ(w, q.w);
}

TEST(Quaternion, TestCopyConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q1 = ds_math::Quaternion(x, y, z, w);
    ds_math::Quaternion q2 = ds_math::Quaternion(q1);

    EXPECT_EQ(q1.x, q2.x);
    EXPECT_EQ(q1.y, q2.y);
    EXPECT_EQ(q1.z, q2.z);
    EXPECT_EQ(q1.w, q2.w);
}

TEST(Quaternion, TestCopyAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q1 = ds_math::Quaternion(x, y, z, w);
    ds_math::Quaternion q2 = q1;

    EXPECT_EQ(q1.x, q2.x);
    EXPECT_EQ(q1.y, q2.y);
    EXPECT_EQ(q1.z, q2.z);
    EXPECT_EQ(q1.w, q2.w);
}

TEST(Quaternion, TestIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);

    EXPECT_EQ(x, q[0]);
    EXPECT_EQ(y, q[1]);
    EXPECT_EQ(z, q[2]);
    EXPECT_EQ(w, q[3]);
}

TEST(Quaternion, TestConstIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    const ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);

    EXPECT_EQ(x, q[0]);
    EXPECT_EQ(y, q[1]);
    EXPECT_EQ(z, q[2]);
    EXPECT_EQ(w, q[3]);
}

TEST(Quaternion, TestEquivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q1 = ds_math::Quaternion(x, y, z, w);
    ds_math::Quaternion q2 = ds_math::Quaternion(x, y, z, w);

    EXPECT_TRUE(q1 == q2);

    q1.x = 0.0f;

    EXPECT_FALSE(q1 == q2);
}

TEST(Quaternion, TestInequivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float w = 2e-2f;

    ds_math::Quaternion q1 = ds_math::Quaternion(x, y, z, w);
    ds_math::Quaternion q2 = ds_math::Quaternion(x, y, z, w);

    EXPECT_FALSE(q1 != q2);

    q1.x = 0.0f;

    EXPECT_TRUE(q1 != q2);
}

TEST(Quaternion, TestDotProduct)
{
    ds_math::Quaternion vec1 = ds_math::Quaternion(3.0f, 1.0f, -23.03f, 1.0f);
    ds_math::Quaternion vec2 =
        ds_math::Quaternion(-3.4f, 100.0f, -3.897f, 0.5f);
    float expectedResult = 180.04791f;

    EXPECT_EQ(expectedResult, ds_math::Quaternion::Dot(vec1, vec2));
}

TEST(Quaternion, TestMagnitude)
{
    float x = 2;
    float y = 2;
    float z = -99.0f;
    float w = 0.3f;

    ds_math::Quaternion vec = ds_math::Quaternion(x, y, z, w);

    EXPECT_TRUE(
        fabs(vec.Magnitude() - sqrt((x * x) + (y * y) + (z * z) + (w * w))) <=
        ds_math::FLOAT_ACCURACY);
    EXPECT_TRUE(fabs(ds_math::Quaternion::Magnitude(vec) -
                     sqrt((x * x) + (y * y) + (z * z) + (w * w))) <=
                ds_math::FLOAT_ACCURACY);
}

TEST(Quaternion, TestNormalize)
{
    float x = 1.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);

    q.Normalize();
    EXPECT_EQ(ds_math::Quaternion(x, y, z, w), q);

    x = 993.0f;
    y = 0.2f;
    z = -3.321f;
    w = 38.8f;

    q = ds_math::Quaternion(x, y, z, w);
    float mag = q.Magnitude();
    q.Normalize();

    EXPECT_EQ(ds_math::Quaternion(x / mag, y / mag, z / mag, w / mag), q);

    q = ds_math::Quaternion(x, y, z, w);
    ds_math::Quaternion normalized = ds_math::Quaternion::Normalize(q);
    q.Normalize();

    EXPECT_EQ(q, normalized);
}

TEST(Quaternion, TestInvert)
{
    float x = -13.0f;
    float y = 2.0f;
    float z = 0.0f;
    float w = -10.9f;
    float invNorm = 1.0f / ((x * x) + (y * y) + (z * z) + (w * w));
    float xExpect = -x * invNorm;
    float yExpect = -y * invNorm;
    float zExpect = -z * invNorm;
    float wExpect = w * invNorm;

    ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);
    q.Invert();

    EXPECT_EQ(xExpect, q.x);
    EXPECT_EQ(yExpect, q.y);
    EXPECT_EQ(zExpect, q.z);
    EXPECT_EQ(wExpect, q.w);

    q = ds_math::Quaternion(x, y, z, w);
    q = ds_math::Quaternion::Invert(q);

    EXPECT_EQ(xExpect, q.x);
    EXPECT_EQ(yExpect, q.y);
    EXPECT_EQ(zExpect, q.z);
    EXPECT_EQ(wExpect, q.w);

    q = ds_math::Quaternion(x, y, z, w);
    q = -q;

    EXPECT_EQ(xExpect, q.x);
    EXPECT_EQ(yExpect, q.y);
    EXPECT_EQ(zExpect, q.z);
    EXPECT_EQ(wExpect, q.w);
}

TEST(Quaternion, TestQuaternionMultiplication)
{
    ds_math::Quaternion q1 = ds_math::Quaternion(1.0f, 23.33f, -0.32f, 3.3f);
    ds_math::Quaternion q2 = ds_math::Quaternion(0.995f, -0.2f, 0.0f, 0.0f);
    ds_math::Quaternion expectedResult =
        ds_math::Quaternion(3.2195f, -0.9784f, -23.41335f, 3.671f);

    EXPECT_EQ(expectedResult, q1 * q2);
}

TEST(Quaternion, TestOutputOperator)
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
    float w = 0.0f;
    std::stringstream stream, resultStream;
    ds_math::Quaternion q = ds_math::Quaternion(x, y, z, w);

    resultStream << "{" << x << ", " << y << ", " << z << ", " << w << "}";
    stream << q;

    EXPECT_EQ(resultStream.str(), stream.str());
}
