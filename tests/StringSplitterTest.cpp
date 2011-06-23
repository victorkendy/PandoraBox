
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "pbge/utils/StringSplitter.h"

using std::string;
using std::vector;

TEST(StringSplitter, willSplitStringsOnSeparator) {
    vector<string> tokens;
    string text = "I'm a very    nice string!!";

    pbge::StringSplitter splitter;
    splitter.split(text, tokens);
    EXPECT_EQ("I'm", tokens.at(0));
    EXPECT_EQ("a", tokens.at(1));
    EXPECT_EQ("very", tokens.at(2));
    EXPECT_EQ("nice", tokens.at(3));
    EXPECT_EQ("string!!", tokens.at(4));
    EXPECT_EQ(5, tokens.size());
}

TEST(StringSplitter, acceptsMultipleSeparators) {
    vector<string> tokens;
    string text = "multi,separator;text ,;ok!";

    pbge::StringSplitter splitter(" ,;");
    splitter.split(text, tokens);
    EXPECT_EQ("multi", tokens.at(0));
    EXPECT_EQ("separator", tokens.at(1));
    EXPECT_EQ("text", tokens.at(2));
    EXPECT_EQ("ok!", tokens.at(3));
    EXPECT_EQ(4, tokens.size());
}

TEST(StringSplitter, worksWhenTextEndsWithSeparator) {
    vector<string> tokens;
    string text = "multi,separator;text ,;ok!    ";

    pbge::StringSplitter splitter;
    splitter.split(text, tokens);
    EXPECT_EQ("multi,separator;text", tokens.at(0));
    EXPECT_EQ(",;ok!", tokens.at(1));
    EXPECT_EQ(2, tokens.size());
}

TEST(StringSplitter, worksWhenTextBeginsWithSeparator) {
    vector<string> tokens;
    string text = "     multi,separator;text ,;ok!";

    pbge::StringSplitter splitter;
    splitter.split(text, tokens);
    EXPECT_EQ("multi,separator;text", tokens.at(0));
    EXPECT_EQ(",;ok!", tokens.at(1));
    EXPECT_EQ(2, tokens.size());
}
