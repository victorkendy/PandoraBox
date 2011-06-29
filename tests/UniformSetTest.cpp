#include <gtest/gtest.h>

#include "pbge/gfx/UniformSet.h"

TEST(UniformSet, searchShouldReturnNULLIfTheUniformIsNotDefined) {
    pbge::UniformSet set;
    EXPECT_EQ(NULL, set.searchValue(pbge::UniformInfo("bla", pbge::FLOAT)));
}

TEST(UniformSet, getFloatMustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformFloat * f = set.getFloat("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getFloatVec2MustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformFloatVec2 * f = set.getFloatVec2("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getFloatVec3MustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformFloatVec3 * f = set.getFloatVec3("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getFloatVec4MustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformFloatVec4 * f = set.getFloatVec4("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getSampler1DMustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformSampler1D * f = set.getSampler1D("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getSampler2DMustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformSampler2D * f = set.getSampler2D("name");
    EXPECT_FALSE(f == NULL);
}

TEST(UniformSet, getFloatMat4MustCreateANewInstanceOfTheValueOnFirstGet) {
    pbge::UniformSet set;
    pbge::UniformMat4 * f = set.getFloatMat4("name");
    EXPECT_FALSE(f == NULL);
}

// still missing some types

TEST(UniformSet, searchShouldReturnTheValueCreatedByGet) {
    pbge::UniformSet set;
    pbge::UniformFloat * f = set.getFloat("name");
    EXPECT_EQ(f, set.searchValue(pbge::UniformInfo("name", pbge::FLOAT)));
}

TEST(UniformSet, secondGetShouldReturnTheInstanceCreatedByFirstGet) {
    pbge::UniformSet set;
    pbge::UniformFloat * f = set.getFloat("name");
    pbge::UniformFloat * f2 = set.getFloat("name");
    EXPECT_EQ(f, f2);
}