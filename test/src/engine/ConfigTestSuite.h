#include <cmath>

#include "gtest/gtest.h"

#include "engine/Config.h"

TEST(Config, LoadFile)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));
}

TEST(Config, GetUnsignedInt)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_TRUE(cfg.GetUnsignedInt("test", &uint));
    EXPECT_EQ(8, uint);
}

TEST(Config, GetUnsignedIntNested)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_TRUE(cfg.GetUnsignedInt("Video.redBits", &uint));
    EXPECT_EQ(8, uint);
}

TEST(Config, GetUnsignedIntNestedIncorrect)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_FALSE(cfg.GetUnsignedInt("test.redBits", &uint));
}

TEST(Config, FailingToGetValueDoesNotModifyUnsignedInt)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_FALSE(cfg.GetUnsignedInt("test.redBits", &uint));
    EXPECT_EQ(0, uint);

    uint = 100;
    EXPECT_FALSE(cfg.GetUnsignedInt("test.redBits", &uint));
    EXPECT_EQ(100, uint);
}

TEST(Config, TwoLevelsOfNesting)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_TRUE(cfg.GetUnsignedInt("Video.Dimensions.width", &uint));
    EXPECT_EQ(800, uint);
}

TEST(Config, GetUintIncorrectType)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    unsigned int uint = 0;
    EXPECT_FALSE(cfg.GetUnsignedInt("Video.fullscreen", &uint));
    EXPECT_EQ(0, uint);
}

TEST(Config, GetString)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::string str;
    EXPECT_TRUE(cfg.GetString("Video.title", &str));
    EXPECT_EQ(str, std::string("Hello world!"));
}

TEST(Config, GetStringIncorrect)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::string str;
    EXPECT_FALSE(cfg.GetString("Video.title2", &str));
}

TEST(Config, GetStringIncorrectDoesNotModifyOriginal)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::string str;
    EXPECT_FALSE(cfg.GetString("Video.title2", &str));
    EXPECT_EQ(0, str.size());
}

TEST(Config, GetStringIncorrectType)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::string str;
    EXPECT_FALSE(cfg.GetString("Video.fullscreen", &str));
    EXPECT_EQ(0, str.size());
}

TEST(Config, GetBool)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    bool boolean;
    EXPECT_TRUE(cfg.GetBool("Video.fullscreen", &boolean));
    EXPECT_EQ(false, boolean);
}

TEST(Config, GetBoolIncorrect)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    bool boolean;
    EXPECT_FALSE(cfg.GetBool("Video.fullscreen2", &boolean));
}

TEST(Config, GetBoolIncorrectDoesNotModifyOriginal)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    bool boolean = false;
    EXPECT_FALSE(cfg.GetBool("Video.fullscreen2", &boolean));
    EXPECT_EQ(false, boolean);
}

TEST(Config, GetBoolIncorrectType)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    bool boolean = false;
    EXPECT_FALSE(cfg.GetBool("Video.redBits", &boolean));
    EXPECT_EQ(false, boolean);
}

TEST(Config, GetObjectKeys)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::vector<std::string> keys = cfg.GetObjectKeys("Input");
    EXPECT_EQ(3, keys.size());
    EXPECT_EQ("InputContextName", keys[0]);
    EXPECT_EQ("Empty", keys[1]);
    EXPECT_EQ("Default", keys[2]);
}

TEST(Config, GetObjectKeysNested)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::vector<std::string> keys = cfg.GetObjectKeys("Input.InputContextName");
    EXPECT_EQ(1, keys.size());
    EXPECT_EQ("keyName", keys[0]);
}

TEST(Config, GetObjectKeysNotAnObject)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::vector<std::string> keys = cfg.GetObjectKeys("Video.profile");
    EXPECT_EQ(0, keys.size());
}

TEST(Config, GetObjectKeysEmptyObject)
{
    ds::Config cfg;

    EXPECT_TRUE(cfg.LoadFile("../assets/config.json"));

    std::vector<std::string> keys = cfg.GetObjectKeys("Input.Empty");
    EXPECT_EQ(0, keys.size());
}
