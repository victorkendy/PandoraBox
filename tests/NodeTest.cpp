#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

#include "mocks/MockOpenGL.h"
#include "mocks/MockVisitors.h"

class NodeTest : public testing::Test {
public:
    math3d::matrix44 *m, *t;
    pbge::TransformationNode * transformationNode;
    pbge::UpdaterVisitor * updater;
    MockOpenGL ogl;

    void SetUp() {
        transformationNode = new pbge::TransformationNode;
        m = new math3d::matrix44(1.0f, 1.1f, 1.2f, 1.3f, 2.0f, 2.1f, 2.2f, 2.3f,
                             3.0f, 3.1f, 3.2f, 3.3f, 4.0f, 4.1f, 4.2f, 4.3f);

        t = new math3d::matrix44(5.55f, 6.3f, 4.44f, 0.0f, 9.0f, 10.0f, 11.0f, 12.0f,
                             1.0f , 2.0f, 3.0f , 4.0f, 1.0f, 1.0f , 1.0f , 1.0f);
        transformationNode->setTransformationMatrix(*t);

        updater = new pbge::UpdaterVisitor;
    }

    ~NodeTest() {
        delete transformationNode;
        delete m;
        delete t;
        delete updater;
    }
};


TEST_F(NodeTest, transformationNodeUpdaterComposesTransformations) {
    updater->pushTransformation(*m);
    transformationNode->updatePass(updater, NULL);
    EXPECT_CALL(ogl, loadModelMatrix(::testing::Eq(::testing::ByRef((*m) * (*t)))));
    MockRenderVisitor renderVisitor;
    transformationNode->renderPass(&renderVisitor, &ogl);
}


//////////////////////////////////////////////////////////////////////////
//
//class UpdaterVisitorTest : public testing::Test {
//public:
//    MockTransformationNode n1, n2, n3;
//    pbge::UpdaterVisitor * visitor;
//
//    virtual void SetUp() {
//        visitor = new pbge::UpdaterVisitor;
//        
//        n1.getChilds()->push_back(&n2);
//        n1.getChilds()->push_back(&n3);
//    }
//
//    ~UpdaterVisitorTest() {
//        //delete visitor;
//    }
//};
//
//TEST_F(UpdaterVisitorTest, visitorCallsExactlyOnceTheUpdateOfEveryNodeOfTheGraph) {
//    EXPECT_CALL(n1, update(NULL)).Times(1).WillOnce(::testing::Return((math3d::matrix44 *)NULL));
//    EXPECT_CALL(n2, update(NULL)).Times(1).WillOnce(::testing::Return((math3d::matrix44 *)NULL));
//    EXPECT_CALL(n3, update(NULL)).Times(1).WillOnce(::testing::Return((math3d::matrix44 *)NULL));
//    visitor->visit(&n1);
//}
//
//TEST_F(UpdaterVisitorTest, visitorPushAndPopTheTransformationsOnTheDFS) {
//    math3d::matrix44 * m1, *m2, *m3;
//    m1 = (math3d::matrix44 *)1;
//    m2 = (math3d::matrix44 *)2;
//    m3 = (math3d::matrix44 *)3;
//    EXPECT_CALL(n1, update(NULL)).Times(1).WillOnce(::testing::Return(m1));
//    EXPECT_CALL(n2, update(m1)).Times(1).WillOnce(::testing::Return(m2));
//    EXPECT_CALL(n3, update(m1)).Times(1).WillOnce(::testing::Return(m3));
//
//    visitor->visit(&n1);
//}
//
