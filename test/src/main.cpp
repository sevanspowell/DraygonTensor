#include "gtest/gtest.h"

#include "engine/common/CommonTestSuite.h"
#include "engine/common/StreamBufferTestSuite.h"
#include "engine/ConfigTestSuite.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
