#include <cstdio>

#include "gtest/gtest.h"

#include "engine/json/Json.h"

using namespace ds;

TEST(Json, ParseObject)
{
    const char *json = "{ \"test\": \"Hello world\" }";

    JsonObject object;
    json::parseObject(json, &object);

    EXPECT_TRUE(object["test"] != nullptr);
}

TEST(Json, ParseString)
{
    const char *json = "{ \"test\": \"Hello world\" }";

    JsonObject object;
    json::parseObject(json, &object);

    std::string str;
    json::parseString(object["test"], &str);
    EXPECT_TRUE(strcmp(str.c_str(), "Hello world") == 0);
}
