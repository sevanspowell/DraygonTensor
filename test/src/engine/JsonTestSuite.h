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

TEST(Json, ParseNumber)
{
    const char *json =
        "{ \"number1\": 0.4, \"number2\": -0.1, \"number3\": 100000000 }";

    JsonObject object;
    json::parseObject(json, &object);

    EXPECT_EQ(0.4f, json::parseFloat(object["number1"]));
    EXPECT_EQ(-0.1f, json::parseFloat(object["number2"]));
    EXPECT_EQ(100000000, json::parseInt(object["number3"]));
    EXPECT_EQ(-0.1, json::parseNumber(object["number2"]));
}

TEST(Json, ParseBool)
{
    const char *json = "{ \"lockMouse?\": false, \"windowed\": true }";

    JsonObject object;
    json::parseObject(json, &object);

    EXPECT_EQ(false, json::parseBool(object["lockMouse?"]));
    EXPECT_EQ(true, json::parseBool(object["windowed"]));
}

TEST(Json, ParseArray)
{
    const char *json = "{ \"myArray\": [0.4, 2] }";

    JsonObject object;
    json::parseObject(json, &object);

    JsonArray array;
    json::parseArray(object["myArray"], &array);

    EXPECT_EQ(0.4f, json::parseFloat(array[0]));
    EXPECT_EQ(2, json::parseInt(array[1]));
}

TEST(Json, ParseNested)
{
    const char *json = "{ \"affordance\": [ { \"sit\": 0.4}, { \"drink\": 1.0 "
                       "}, { \"rest\": 0.0 } ] }";

    const char *names[] = {"sit", "drink", "rest"};
    float values[] = {0.4f, 1.0f, 0.0f};

    JsonObject object;
    json::parseObject(json, &object);

    JsonArray affordances;
    json::parseArray(object["affordance"], &affordances);

    for (unsigned int i = 0; i < affordances.size(); ++i)
    {
        JsonObject affordance;
        json::parseObject(affordances[i], &affordance);
        EXPECT_EQ(values[i], json::parseFloat(affordance[names[i]]));
    }
}

TEST(Json, Type)
{
    const char *json = "{"
                       " \"string\": \"myString\", "
                       " \"number\": 999324.3, "
                       " \"bool\": true, "
                       " \"array\": [0.4, -230, -3.14, \"false\", false] "
                       "}";

    JsonObject object;
    json::parseObject(json, &object);

    EXPECT_EQ(JsonValueType::Enum::Object, json::type(json));
    EXPECT_EQ(JsonValueType::Enum::String, json::type(object["string"]));
    EXPECT_EQ(JsonValueType::Enum::Number, json::type(object["number"]));
    EXPECT_EQ(JsonValueType::Enum::Bool, json::type(object["bool"]));
    EXPECT_EQ(JsonValueType::Enum::Array, json::type(object["array"]));
}
