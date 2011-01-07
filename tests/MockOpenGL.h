

#ifndef PBGE_TESTS_MOCKOPENGL_H_
#define PBGE_TESTS_MOCKOPENGL_H_

#include <gmock/gmock.h>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Buffer.h"

class MockOpenGL : public pbge::OpenGL {
public:
    MOCK_METHOD1(setMatrixMode, void(GLenum mode));
    MOCK_METHOD1(loadViewMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadProjectionMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadModelMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD0(updateState, void());
    MOCK_METHOD0(uploadProjection, void());
    MOCK_METHOD3(createBuffer, pbge::Buffer*(size_t _size, GLenum _usage, GLenum _target));
    MOCK_METHOD1(activeTexture, void(GLenum textureUnit));
    MOCK_METHOD2(alphaFunc, void(GLenum func, GLclampf ref));
    MOCK_METHOD2(attachShader, void(GLuint program, GLuint shader));
    MOCK_METHOD1(begin, void(GLenum mode));
    MOCK_METHOD2(beginQuery, void(GLenum target, GLuint id));
    MOCK_METHOD3(bindAttribLocation, void(GLuint program, GLuint index, const GLchar* name));
    MOCK_METHOD2(bindBuffer, void(GLenum target, GLuint buffer));
    MOCK_METHOD2(bindTexture, void(GLenum target, GLuint texture));
    MOCK_METHOD4(bufferData, void(GLenum target, GLsizeiptr size, GLvoid * data, GLenum usage));
    MOCK_METHOD4(bufferSubData, void(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data));
    MOCK_METHOD1(clear, void(GLbitfield mask));
    MOCK_METHOD4(clearColor, void(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
    MOCK_METHOD1(clearDepth, void(GLclampd depth));
    MOCK_METHOD1(clearStencil, void(GLint s));
    MOCK_METHOD1(clientActiveTexture, void(GLenum textureUnit));
    MOCK_METHOD2(clipPlane, void(GLenum plane, const GLdouble * equation));
    MOCK_METHOD4(colorPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD2(deleteBuffers, void(GLsizei n, GLuint * buffers));
    MOCK_METHOD1(disable, void(GLenum mode));
    MOCK_METHOD1(enable, void(GLenum mode));
    MOCK_METHOD1(enableClientState, void(GLenum cap));
    MOCK_METHOD2(genBuffers, void(GLsizei n, GLuint * buffers));
    MOCK_METHOD2(getFloatv, void(GLenum pname, GLfloat * params));
    MOCK_METHOD2(getIntegerv, void(GLenum pname, GLint * params));
    MOCK_METHOD1(getString, const char *(GLenum name));
    MOCK_METHOD1(loadMatrix, void(GLfloat * matrix));
    MOCK_METHOD2(mapBuffer, void*(GLenum target, GLenum access));
    MOCK_METHOD1(matrixMode, void(GLenum mode));
    MOCK_METHOD3(normalPointer, void(GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(secondaryColorPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(texCoordPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(vertexPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(viewport, void(GLint x, GLint y, GLint w, GLint h));
    
    MOCK_METHOD2(genFramebuffersEXT, void(GLsizei n, GLuint * buffers));
    MOCK_METHOD2(deleteFramebuffersEXT, void(GLsizei n, GLuint * buffers));
};

class MockBuffer : public pbge::Buffer {
public:
    MOCK_METHOD0(map, void*());
};

#endif //PBGE_TESTS_MOCKOPENGL_H_
