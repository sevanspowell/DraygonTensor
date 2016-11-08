include <cmath>

#include "gtest/gtest.h"

#include "engine/common/Common.h"

TEST(TokenizeString, CorrectNumberOfTokens)
{
    std::string string("a.b.c.d");

    std::vector<std::string> tokens = ds_com::TokenizeString('.', string);

    EXPECT_EQ(4, tokens.size());
}

TEST(TokenizeString, CorrectTokens)
{
    std::string string("a.b.c.d");

    std::vector<std::string> tokens = ds_com::TokenizeString('.', string);

    EXPECT_EQ("a", tokens[0]);
    EXPECT_EQ("b", tokens[1]);
    EXPECT_EQ("c", tokens[2]);
    EXPECT_EQ("d", tokens[3]);
}

TEST(TokenizeString, CorrectNumberOfTokensComplex)
{
    std::string string("My\\Ridiculous.table.idea.format*.**");

    std::vector<std::string> tokens = ds_com::TokenizeString('.', string);

    EXPECT_EQ(5, tokens.size());
}

TEST(TokenizeString, CorrectTokensComplex)
{
    std::string string("My\\Ridiculous.table.idea.format*.**");

    std::vector<std::string> tokens = ds_com::TokenizeString('.', string);

    EXPECT_EQ("My\\Ridiculous", tokens[0]);
    EXPECT_EQ("table", tokens[1]);
    EXPECT_EQ("idea", tokens[2]);
    EXPECT_EQ("format*", tokens[3]);
    EXPECT_EQ("**", tokens[4]);
}
