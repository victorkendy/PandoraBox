#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

#include "mocks/MockGraphicAPI.h"
#include "mocks/MockVisitors.h"
#include "mocks/MockNode.h"

class NodeTest : public testing::Test {
public:
    math3d::matrix44 *m, *t;
    pbge::TransformationNode * transformationNode;
    pbge::UpdaterVisitor * updater;
    MockGraphicAPI * ogl;
    MockRenderVisitor * renderVisitor;

    void SetUp() {
        ogl = new MockGraphicAPI;
        transformationNode = new pbge::TransformationNode;
        m = new math3d::matrix44(1.0f, 1.1f, 1.2f, 1.3f, 2.0f, 2.1f, 2.2f, 2.3f,
                             3.0f, 3.1f, 3.2f, 3.3f, 4.0f, 4.1f, 4.2f, 4.3f);

        t = new math3d::matrix44(5.55f, 6.3f, 4.44f, 0.0f, 9.0f, 10.0f, 11.0f, 12.0f,
                             1.0f , 2.0f, 3.0f , 4.0f, 1.0f, 1.0f , 1.0f , 1.0f);
        transformationNode->setTransformationMatrix(*t);

        updater = new pbge::UpdaterVisitor;
        renderVisitor = new MockRenderVisitor;
    }

    ~NodeTest() {
        delete ogl;
        delete transformationNode;
        delete m;
        delete t;
        delete updater;
        delete renderVisitor;

        math3d::print_internal_memory_page_info();
    }
};

TEST_F(NodeTest, postRenderPassRestauresThePreviousTransformation) {
    updater->pushTransformation(*m);
    transformationNode->updatePass(updater, NULL);
    EXPECT_TRUE((*m * *t) == updater->getCurrentTransformation());
    transformationNode->postUpdatePass(updater, NULL);
    EXPECT_TRUE(*m == updater->getCurrentTransformation());
}

TEST_F(NodeTest, transformationNodeUpdaterComposesTransformationsForRenderPass) {
    updater->pushTransformation(*m);
    transformationNode->updatePass(updater, NULL);
    transformationNode->postUpdatePass(updater, NULL);
    EXPECT_CALL(*ogl, loadModelMatrix(::testing::Eq(::testing::ByRef((*m) * (*t)))));
    transformationNode->renderPass(renderVisitor, ogl);
}

TEST_F(NodeTest, transformationNodeUpdaterComposesTransformationsForDepthPass) {
    updater->pushTransformation(*m);
    transformationNode->updatePass(updater, NULL);
    transformationNode->postUpdatePass(updater, NULL);
    EXPECT_CALL(*ogl, loadModelMatrix(::testing::Eq(::testing::ByRef((*m) * (*t)))));
    transformationNode->depthPass(renderVisitor, ogl);
}

//////////////////////////////////////////////////////////////////////////

class UpdaterVisitorTest : public testing::Test {
public:
    pbge::UpdaterVisitor * visitor;
    MockNode root, child1, child2;

    virtual void SetUp() {
        visitor = new pbge::UpdaterVisitor;
        root.addChild(&child1);
        root.addChild(&child2);
    }

    ~UpdaterVisitorTest() {
        delete visitor;
        math3d::print_internal_memory_page_info();
    }
};

TEST_F(UpdaterVisitorTest, updaterVisitorCallsUpdatePassAndPostUpdatePassFromANodeAndAllItsChilds) {
    EXPECT_CALL(root, updatePass(visitor, NULL));
    EXPECT_CALL(root, postUpdatePass(visitor, NULL));

    EXPECT_CALL(child1, updatePass(visitor, NULL));
    EXPECT_CALL(child1, postUpdatePass(visitor, NULL));

    EXPECT_CALL(child2, updatePass(visitor, NULL));
    EXPECT_CALL(child2, postUpdatePass(visitor, NULL));
    visitor->visit(&root, NULL);
}
