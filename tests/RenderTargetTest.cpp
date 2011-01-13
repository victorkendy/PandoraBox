
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "pbge/gfx/RenderTarget.h"
#include "mocks/MockOpenGL.h"


TEST(ViewportTest, setRenderTargetCallsOpenGLViewportCorrectly) {
    GLint w=100, h=100, x=10, y=20;
    pbge::Viewport viewport(w,h,x,y);
    MockOpenGL ogl;
    EXPECT_CALL(ogl, viewport(x,y,w,h));
    viewport.setRenderTarget(&ogl);
};
