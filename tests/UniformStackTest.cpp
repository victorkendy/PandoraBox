#include <gtest/gtest.h>

#include "pbge/gfx/UniformStack.h"
#include "pbge/gfx/UniformSet.h"

class UniformStackTest : public ::testing::Test {
public:
    pbge::UniformStack stack;
    pbge::UniformSet set1;
    pbge::UniformSet set2;
    void SetUp() {
        
    }
};

TEST_F(UniformStackTest, shouldFindAExistingUniform) {
    pbge::UniformFloat * f1 = set1.getFloat("name1");
    pbge::UniformFloat * f2 = set2.getFloat("name2");
    stack.push(&set1);
    stack.push(&set2);
    ASSERT_EQ(f1, stack.findUniform(pbge::UniformInfo("name1", pbge::FLOAT)));
    ASSERT_EQ(f2, stack.findUniform(pbge::UniformInfo("name2", pbge::FLOAT)));
}

TEST_F(UniformStackTest, shouldReturnNULLIfUniformIsNotFound) {
    pbge::UniformFloat * f1 = set1.getFloat("name1");
    pbge::UniformFloat * f2 = set2.getFloat("name2");
    stack.push(&set1);
    stack.push(&set2);
    ASSERT_EQ(NULL, stack.findUniform(pbge::UniformInfo("name3", pbge::FLOAT)));
}

TEST_F(UniformStackTest, shouldSearchForTheClosestUniformOnTheStack) {
    pbge::UniformFloat * f1 = set1.getFloat("name1");
    pbge::UniformFloat * f2 = set2.getFloat("name1");
    stack.push(&set1);
    stack.push(&set2);
    ASSERT_EQ(f2, stack.findUniform(pbge::UniformInfo("name1", pbge::FLOAT)));
}
