

#ifndef mockopengl_h_
#define mockopengl_h_

#include <gmock/gmock.h>

#include "pbge/gfx/OpenGL.h"

class MockOpenGL : public pbge::OpenGL {
public:
    MOCK_METHOD1(loadViewMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadProjectionMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadModelMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD4(viewport, void(GLint x, GLint y, GLint w, GLint h));
    MOCK_METHOD0(uploadModelview, void());
    MOCK_METHOD0(uploadProjection, void());
};


#endif
