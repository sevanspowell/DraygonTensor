#include "gtest/gtest.h"

#include "engine/ConfigTestSuite.h"
#include "engine/common/CommonTestSuite.h"
#include "engine/common/StreamBufferTestSuite.h"
#include "math/Matrix4TestSuite.h"
#include "math/QuaternionTestSuite.h"
#include "math/Vector3TestSuite.h"
#include "math/Vector4TestSuite.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
