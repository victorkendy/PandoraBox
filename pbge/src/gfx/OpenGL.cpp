#include <GL/glew.h>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Buffer.h"


using namespace pbge;

OpenGL::OpenGL() {
    GLint initialMatrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
    currentMatrixMode = initialMatrixMode;
    matrices[2] = math3d::identity44;
}

void OpenGL::setMatrixMode(GLenum mode) {
    if(currentMatrixMode != mode)
        glMatrixMode(mode);
    currentMatrixMode = mode;
}

void OpenGL::uploadModelview() {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((matrices[0]*matrices[2]).transpose());
}

void OpenGL::uploadProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrices[1].transpose());
}

Buffer * OpenGL::createBuffer(size_t _size, GLenum _usage, GLenum _target) {
    return new Buffer(_size, _usage, _target);
}

void OpenGL::activeTexture(GLenum textureUnit) {
    glActiveTexture(textureUnit);
}

void OpenGL::alphaFunc(GLenum func, GLclampf ref) {
    glAlphaFunc(func, ref);
}

void OpenGL::attachShader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
}

void OpenGL::begin(GLenum mode) {
    glBegin(mode);
}

void OpenGL::beginQuery(GLenum target, GLuint id) {
    glBeginQuery(target, id);
}

void OpenGL::bindAttribLocation(GLuint program, GLuint index, const GLchar* name) {
    glBindAttribLocation(program, index, name);
}

void OpenGL::bindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void OpenGL::bindTexture(GLenum target, GLuint texture) {
    glBindTexture(target, texture);
}

void OpenGL::bufferData(GLenum target, GLsizeiptr size, GLvoid * data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void OpenGL::bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) {
    glBufferSubData(target, offset, size, data);
}

void OpenGL::clear(GLbitfield mask) {
    glClear(mask);
}

void OpenGL::clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    glClearColor(red, green, blue, alpha);
}

void OpenGL::clearDepth(GLclampd depth) {
    glClearDepth(depth);
}

void OpenGL::clearStencil(GLint s) {
    glClearStencil(s);
}

void OpenGL::clientActiveTexture(GLenum textureUnit) {
    glClientActiveTexture(textureUnit);
}

void OpenGL::clipPlane(GLenum plane, const GLdouble * equation) {
    glClipPlane(plane, equation);
}

void OpenGL::colorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glColorPointer(size, type, stride, pointer);
}

void OpenGL::deleteBuffers(GLsizei n, GLuint * buffers) {
    glDeleteBuffers(n, buffers);
}

void OpenGL::disable(GLenum mode) {
    glDisable(mode);
}

void OpenGL::enable(GLenum mode) {
    glEnable(mode);
}

void OpenGL::enableClientState(GLenum cap) {
    glEnableClientState(cap);
}

void OpenGL::genBuffers(GLsizei n, GLuint * buffers) {
    glGenBuffers(n, buffers);
}

void OpenGL::getFloatv(GLenum pname, GLfloat * params) {
    glGetFloatv(pname, params);
}

void OpenGL::getIntegerv(GLenum pname, GLint * params) {
    glGetIntegerv(pname, params);
}

const char * OpenGL::getString(GLenum name) {
    return (const char *)glGetString(name);
}

void OpenGL::loadMatrix(GLfloat * matrix) {
    glLoadMatrixf(matrix);
}

void * OpenGL::mapBuffer(GLenum target, GLenum access) {
    return glMapBuffer(target, access);
}

void OpenGL::matrixMode(GLenum mode) {
    glMatrixMode(mode);
}

void OpenGL::normalPointer(GLenum type, GLsizei stride, GLvoid * pointer) {
    glNormalPointer(type, stride, pointer);
}

void OpenGL::secondaryColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glSecondaryColorPointer(size, type, stride, pointer);
}

void OpenGL::texCoordPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glTexCoordPointer(size, type, stride, pointer);
}

void OpenGL::vertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glVertexPointer(size, type, stride, pointer);
}

void OpenGL::viewport(GLint x, GLint y, GLint w, GLint h) {
    glViewport(x,y,w,h);
}



void OpenGL::genFramebuffersEXT(GLsizei n, GLuint * buffers) {
    glGenFramebuffersEXT(n, buffers);
}

void OpenGL::deleteFramebuffersEXT(GLsizei n, GLuint * buffers) {
    glDeleteFramebuffersEXT(n, buffers);
}
