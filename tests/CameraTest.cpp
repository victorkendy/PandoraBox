
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "pbge/gfx/Camera.h"

#include "mocks/MockOpenGL.h"
//#include "mocks/MockTransformationNode.h"
//#include "mocks/MockRenderTarget.h"

#ifndef M_PI
#define M_PI 3.1415f
#endif

using ::testing::_;

class FrustumTest : public testing::Test {
public:
    MockOpenGL ogl;
    pbge::Frustum frustum;

    FrustumTest() {}
    ~FrustumTest() {}
};

TEST_F(FrustumTest, orthogonalProjectionIsCalculatedCorrectly) {
    math3d::matrix44 m;
    // See http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
    m = math3d::matrix44(2.0f/7.0f, 0.0f, 0.0f, -1.0f/7.0f,
                         0.0f, 1.0f/3.0f, 0.0f, -2.0f/3.0f,
                         0.0f, 0.0f, -2.0f/100.0f, -8.0f/10.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);
    frustum.setOrtho(-3, 4, -1, 5, -10, 90);
    EXPECT_CALL(ogl, loadProjectionMatrix(m)).Times(1);
    frustum.loadProjection(&ogl);
}

TEST_F(FrustumTest, perspectiveProjectionIsCalculatedCorrectly) {
    math3d::matrix44 m;
    float aspect = 1.5, angler = M_PI/3.0f, angled = 60.0f, near = 10.0f, far = 90.0f;
    float f = 1.0f / tan(angler / 2.0f);
    // See http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
    m = math3d::matrix44(f/aspect, 0.0f, 0.0f,                  0.0f,
                         0.0f,     f,    0.0f,                  0.0f,
                         0.0f,     0.0f, (far+near)/(near-far), 2.0f * far * near/(near - far),
                         0.0f,     0.0f, -1.0f,                 0.0f);

    frustum.setPerspective(angled, aspect, near, far);
    EXPECT_CALL(ogl, loadProjectionMatrix(m)).Times(1);
    frustum.loadProjection(&ogl);
}

TEST_F(FrustumTest, frustumIsCalculatedCorrectly) {
    math3d::matrix44 m;
    float l = 1.5f, r = 3.5f, b = -10.0f, t = 25.0f, n = 1.0f, f = 100.0f;
    float A = (r+l)/(r-l), B = (t+b)/(t-b), C = -(f+n)/(f-n), D = -2.0f*n*f/(f-n);
    // See http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml
    m = math3d::matrix44(2.0f*n/(r-l), 0.0f,         A,     0.0f,
                         0.0f,         2.0f*n/(t-b), B,     0.0f,
                         0.0f,         0.0f,         C,     D,
                         0.0f,         0.0f,         -1.0f, 0.0f);

    frustum.setFrustum(l,r,b,t,n,f);
    EXPECT_CALL(ogl, loadProjectionMatrix(m)).Times(1);
    frustum.loadProjection(&ogl);
}

TEST_F(FrustumTest, customProjectionMatrixIsLoadedCorrectly) {
    math3d::matrix44 m;
    m = math3d::matrix44(0.701707f, 0.0f, -0.707107f, 0.0f,
                             0.408248f, -0.816497f, 0.408248f, 0.0f,
                             -0.57735f, -0.57735f, -0.57735f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f);

    frustum.setProjectionMatrix(m);
    EXPECT_CALL(ogl, loadProjectionMatrix(m)).Times(1);
    frustum.loadProjection(&ogl);
}

/*

class CameraTest : public testing::Test {
public:
    MockOpenGL ogl;
    MockTransformationNode node;
    math3d::matrix44 m, nt;
    math3d::vector4 f, u;

    CameraTest() {
        nt = math3d::identity44;
        nt[0][3] = 1.0f;
        nt[1][3] = 2.0f;
        nt[2][3] = -3.0f;
        m = math3d::matrix44(0.701707f, 0.0f, -0.707107f, 0.0f,
                             0.408248f, -0.816497f, 0.408248f, 0.0f,
                             -0.57735f, -0.57735f, -0.57735f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f);

        f = math3d::vector4(1,1,1);
        u = math3d::vector4(0, -1, 0);

        EXPECT_CALL(ogl, loadProjectionMatrix(_)).Times(1);
    }
    ~CameraTest() {
    }
};

TEST_F (CameraTest, defaultCameraConstructorPointsToNegativeZAndPositiveY) {
    EXPECT_CALL(ogl, loadViewMatrix(math3d::identity44)).Times(1);
    pbge::Camera camera;
    camera.setCamera(&ogl);
}

TEST_F (CameraTest, constructorWithFrontAndUpVectorWorksAsExpected) {
    pbge::Camera camera(u,f);
    EXPECT_CALL(ogl, loadViewMatrix(m)).Times(1);
    camera.setCamera(&ogl);
}

TEST_F (CameraTest, afterLookAtShouldLoadTheRightView) {
    pbge::Camera camera;
    camera.lookAt(u, f);
    EXPECT_CALL(ogl, loadViewMatrix(m)).Times(1);
    camera.setCamera(&ogl);
}

TEST_F (CameraTest, composesTheCameraTransformationWithTheParentTransformationCorrectly) {
    pbge::Camera camera(u,f);
    camera.setParent(&node);
    EXPECT_CALL(node, getTransformationMatrix()).Times(1).WillOnce(::testing::Return(&nt));
    EXPECT_CALL(ogl, loadViewMatrix(m * nt.inverse())).Times(1);
    camera.setCamera(&ogl);
}

TEST_F (CameraTest, setsAndUnsetsTheRenderTargetIfRenderTargetWasSet) {
    MockRenderTarget renderTarget;
    pbge::Camera camera;
    camera.setRenderTarget(&renderTarget);
    EXPECT_CALL(ogl, loadViewMatrix(_)).Times(1);
    EXPECT_CALL(renderTarget, setRenderTarget(&ogl)).Times(1);
    EXPECT_CALL(renderTarget, unsetRenderTarget(&ogl)).Times(1);
    camera.setCamera(&ogl);
    camera.unsetCamera(&ogl);
}
*/