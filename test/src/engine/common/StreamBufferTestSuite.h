#include <cmath>

#include "gtest/gtest.h"

#include "engine/common/StreamBuffer.h"

// Test insert/extract operations
TEST(StreamBuffer, TestInsertExtract)
{
    ds_com::StreamBuffer stream;

    char write = 'a';
    stream.Insert(sizeof(char), &write);

    char read;
    EXPECT_EQ(true, stream.Extract(sizeof(char), &read));

    EXPECT_EQ(write, read);
}

// Insert/read multiple characters
TEST(StreamBuffer, MultipleInsertExtract)
{
    ds_com::StreamBuffer stream;

    char write = 'a';
    stream.Insert(sizeof(char), &write);
    stream.Insert(sizeof(char), &write);

    char read;
    EXPECT_EQ(true, stream.Extract(sizeof(char), &read));
    EXPECT_EQ(write, read);

    // Reset value of read;
    read = '\0';
    EXPECT_EQ(true, stream.Extract(sizeof(char), &read));
    EXPECT_EQ(write, read);
}

// Try to read from an empty buffer
TEST(StreamBuffer, ReadFail)
{
    ds_com::StreamBuffer stream;

    char read;
    EXPECT_EQ(false, stream.Extract(sizeof(char), &read));
}

// Try to read and write a larger structure
TEST(StreamBuffer, InsertExtractLargerStructure)
{
    ds_com::StreamBuffer stream;

    struct Vector3
    {
        float x, y, z;
    };

    Vector3 point = {0.1f, 1.0f, -3.0f};
    point.x = 1.0f;
    point.y = 1.0f;
    point.z = 1.0f;

    stream.Insert(sizeof(Vector3), &point);

    Vector3 extractedPoint = {0.0f, 0.0f, 0.0f};
    EXPECT_EQ(true, stream.Extract(sizeof(Vector3), &extractedPoint));
    EXPECT_EQ(point.x, extractedPoint.x);
    EXPECT_EQ(point.y, extractedPoint.y);
    EXPECT_EQ(point.z, extractedPoint.z);
}

// Test insertion/extraction operators with char type
TEST(StreamBuffer, OperatorsChar)
{
    ds_com::StreamBuffer stream;

    char write = 'a';
    stream << write;

    char read = '\0';
    stream >> read;
    EXPECT_EQ(write, read);
}

// Test insertion/extraction operators with float type
TEST(StreamBuffer, OperatorsFloat)
{
    ds_com::StreamBuffer stream;

    float write = 1.34f;
    stream << write;

    float read = -1.0f;
    stream >> read;
    EXPECT_EQ(write, read);
}

// Test multiple insertions and extractions with operators
TEST(StreamBuffer, OperatorsMultipleInsertionExtraction)
{
    ds_com::StreamBuffer stream;

    float write = -1.34f;
    stream << write << write << write;

    for (int i = 0; i < 3; ++i)
    {
        float read = 0.0f;
        stream >> read;
        EXPECT_EQ(write, read);
    }
}

// Test insertion and extraction of a non-primitive type with operators
TEST(StreamBuffer, OperatorsInsertExtractLargerStructure)
{
    ds_com::StreamBuffer stream;

    struct Vector3
    {
        float x, y, z;
    };

    Vector3 pA = {-1.34f, 3.04f, 0.99f};
    Vector3 pB = {0.0f, 0.0f, 0.0f};

    stream << pA;
    stream >> pB;

    EXPECT_EQ(pA.x, pB.x);
    EXPECT_EQ(pA.y, pB.y);
    EXPECT_EQ(pA.z, pB.z);
}
