#include <gtest/gtest.h>

#include "pbge/gfx/Renderer.h"

#include "mocks/MockModel.h"
#include "mocks/MockOpenGL.h"
#include "mocks/MockTransformationNode.h"
#include "mocks/MockRenderTarget.h"

using ::testing::_;

class MockCamera : public pbge::Camera {
public:
    MOCK_METHOD1(setCamera, void(pbge::OpenGL *ogl));
    MOCK_METHOD1(unsetCamera, void(pbge::OpenGL * ogl));
};

class BasicRendererVisitorTest : public ::testing::Test {
public:
    MockOpenGL ogl;
    MockTransformationNode root, child1, child2;
    MockModel model;
    MockCamera camera;
    MockRenderTarget renderTarget;
    
    BasicRendererVisitorTest() {
        camera.setRenderTarget(&renderTarget);
    }
};

TEST_F(BasicRendererVisitorTest, visitorCallsTheRenderMethodOfTheModelsOfTheNode) {
    MockModelInstance instance1(&model), instance2(&model);
    pbge::BasicRendererVisitor renderer(&ogl);
    renderer.setCamera(&camera);
    math3d::matrix44 m = math3d::identity44;
    root.getChilds()->push_back(&child1);
    root.getChilds()->push_back(&child2);
    child1.addModelInstance(&instance1);
    child2.addModelInstance(&instance1);
    child2.addModelInstance(&instance2);
    EXPECT_CALL(child1, getTransformationMatrix()).Times(1).WillRepeatedly(::testing::Return(&m));
    EXPECT_CALL(child2, getTransformationMatrix()).Times(1).WillRepeatedly(::testing::Return(&m));
    EXPECT_CALL(camera, setCamera(&ogl)).Times(1);
    EXPECT_CALL(camera, unsetCamera(&ogl)).Times(1);
    EXPECT_CALL(instance1, render(&ogl)).Times(2);
    EXPECT_CALL(instance2, render(&ogl)).Times(1);
    EXPECT_CALL(ogl, updateState()).Times(2);
    EXPECT_CALL(ogl, uploadProjection()).Times(1);
    EXPECT_CALL(ogl, loadModelMatrix(m)).Times(2);
    renderer.visit(&root);
}

class RendererTest : public ::testing::Test {
public:
    MockOpenGL ogl;
    MockModel model;
    MockModelInstance * instance;
    MockTransformationNode root;
    pbge::Camera camera1, camera2;
    pbge::SceneManager scene;
    pbge::Renderer * renderer;

    RendererTest() {
        renderer = new pbge::Renderer(&ogl);
        instance = new MockModelInstance(&model);    
        scene.setSceneGraph(&root);
        renderer->setScene(&scene);
    }

};

void testIfTypeIsUpdater(pbge::NodeVisitor * visitor) {
    if(dynamic_cast<pbge::UpdaterVisitor*>(visitor) == NULL)
        FAIL();
}

void testIfTypeIsRenderer(pbge::NodeVisitor * visitor) {
    if(dynamic_cast<pbge::BasicRendererVisitor*>(visitor) == NULL)
        FAIL();
}

TEST_F(RendererTest, rootAcceptShouldBeCalledForUpdateAndOnceForEachCamera) {
    scene.addCamera(&camera1, "teste1");
    scene.addCamera(&camera2, "teste2");
    EXPECT_CALL(root, accept(_)).Times(3).WillOnce(::testing::Invoke(testIfTypeIsUpdater))
                                         .WillRepeatedly(::testing::Invoke(testIfTypeIsRenderer));
    renderer->render();
}

TEST_F(RendererTest, dontBreakIfNoCameraIsSetUp) {
    EXPECT_CALL(root, accept(_)).Times(1);
    renderer->render();
}

TEST_F(RendererTest, dontBreakIfRootNodeIsNotSetUp) {
    scene.setSceneGraph(NULL);
    scene.addCamera(&camera1, "teste1");
    renderer->render();
}

TEST_F(RendererTest, dontBreakIsRootAndCamerasArentSetUp) {
    scene.setSceneGraph(NULL);
    renderer->render();
}