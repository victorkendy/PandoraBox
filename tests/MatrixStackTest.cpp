

#include <gtest/gtest.h>

#include "math3d/math3d.h"

#include "pbge/gfx/MatrixStack.h"

// Should it?
TEST(MatrixStack, shouldReturnIdentityOnUnderflow) {
    pbge::MatrixStack stack;
    stack.pop();
    EXPECT_TRUE(stack.peek() == math3d::identity44);
}

TEST(MatrixStack, shouldReturnIdentityIfNoPushWasExecuted) {
    pbge::MatrixStack stack;
    EXPECT_TRUE(stack.peek() == math3d::identity44);
}

TEST(MatrixStack, shouldReturnThePushedMatrixOnPeek) {
    pbge::MatrixStack stack;
    math3d::matrix44 m = math3d::scaleMatrix(0.5f, 0.25f, 0.55f);
    stack.push(m);
    EXPECT_TRUE(stack.peek() == m);
}

TEST(MatrixStack, matricesShouldBeStacked) {
    pbge::MatrixStack stack;
    math3d::matrix44 m1 = math3d::scaleMatrix(0.5f, 0.25f, 0.55f);
    math3d::matrix44 m2 = math3d::translationMatrix(1,2,3);
    stack.push(m1);
    stack.push(m2);
    EXPECT_TRUE(stack.peek() == m2);
    stack.pop();
    EXPECT_TRUE(stack.peek() == m1);
}

TEST(MatrixStack, afterClearPeekShouldReturnIdentity) {
    pbge::MatrixStack stack;
    math3d::matrix44 m1 = math3d::scaleMatrix(0.5f, 0.25f, 0.55f);
    math3d::matrix44 m2 = math3d::translationMatrix(1,2,3);
    stack.push(m1);
    stack.push(m2);
    stack.clear();
    EXPECT_TRUE(stack.peek() == math3d::identity44);
}

TEST(MatrixStack, complexOperations) {
    pbge::MatrixStack stack;
    math3d::matrix44 m1 = math3d::scaleMatrix(0.5f, 0.25f, 0.55f);
    math3d::matrix44 m2 = math3d::translationMatrix(1,2,3);
    stack.push(m1);
    EXPECT_TRUE(stack.peek() == m1);
    stack.pop();
    EXPECT_TRUE(stack.peek() == math3d::identity44);
    stack.pop();
    stack.pop();
    EXPECT_TRUE(stack.peek() == math3d::identity44);
    stack.push(m1);
    EXPECT_TRUE(stack.peek() == m1);
    stack.push(m2);
    EXPECT_TRUE(stack.peek() == m2);
    stack.clear();
    EXPECT_TRUE(stack.peek() == math3d::identity44);
}

TEST(MatrixStack, allowsPushingALotOfMatrices) {
    pbge::MatrixStack stack;
    for(int i = 0; i < 100; i++) {
        float f = (float)i;
        stack.push(math3d::translationMatrix(f, -f, f));
    }
    for(int i = 99; i >= 0; i--) {
        float f = (float)i;
        EXPECT_TRUE(stack.peek() == math3d::translationMatrix(f, -f, f));
        stack.pop();
    }
    EXPECT_TRUE(stack.peek() == math3d::identity44);

}