#include <GL/glew.h>
#include <iostream>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/OpenGL/GLBuffer.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/OpenGL/GLObjectsFactory.h"

using namespace pbge;

OpenGL::OpenGL() {
    std::cout << "instancing GL" << std::endl;
    matrices = new math3d::matrix44[3];
    matrices[2] = math3d::identity44;
    this->state = NULL;
    storage = new ResourceStorage;
    this->context = NULL;
    this->factory = new GLObjectsFactory(this);
}

OpenGL::~OpenGL() {
    delete [] matrices;
    delete state;
    delete context;
}

void OpenGL::setContext(GLContext * glContext) {
    this->context = glContext;
    if(context != NULL) {
        context->makeCurrent();
        glewInit();
        GLint initialMatrixMode;
        this->getIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
        currentMatrixMode = initialMatrixMode;
        state = new StateSet(this);
    }
}

void OpenGL::releaseContext() {
    if(context != NULL) {
        context->release();
        context = NULL;
    }
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

void OpenGL::updateState() {
    this->uploadModelview();
    this->state->apply(this);
}

GraphicObjectsFactory * OpenGL::getFactory() {
    return this->factory;
}

UniformValue * OpenGL::getUniformValue(const UniformInfo & info) {
    return this->state->getUniformValue(info);
}

UniformValue * OpenGL::searchUniform(const UniformInfo & info) {
    return this->state->searchUniform(info);
}

void OpenGL::enableMode(Mode mode) {
    this->state->enable(mode);
}

void OpenGL::disableDrawBuffer() {
    this->drawBuffer(GL_NONE);
    this->readBuffer(GL_NONE);
}

void OpenGL::enableDrawBuffer(GLenum buffer) {
    this->drawBuffer(buffer);
    this->readBuffer(buffer);
}

TextureUnit * OpenGL::chooseTextureUnit(Texture * texture) {
    return state->chooseTexUnit(texture);
}

const int OpenGL::numberOfTextureUnits() {
    GLint numberOfUnits = -1;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numberOfUnits);
    std::cout << "number of texture units: " << numberOfUnits << std::endl;
    return numberOfUnits;
}

void OpenGL::pushUniforms(UniformSet * uniforms) {
    this->state->pushUniformSet(uniforms);
}

void OpenGL::popUniforms() {
    this->state->popUniformSet();
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

void OpenGL::bindAttribLocation(GLuint program, GLuint index, const GLchar* name) {
    glBindAttribLocation(program, index, name);
}

void OpenGL::blendFunc(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
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

void OpenGL::clientActiveTexture(GLenum textureUnit) {
    glClientActiveTexture(textureUnit);
}

void OpenGL::colorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glColorPointer(size, type, stride, pointer);
}

void OpenGL::compileShader(GLuint shader) {
    glCompileShader(shader);
}

GLuint OpenGL::createProgram(){
    return glCreateProgram();
}

GLuint OpenGL::createShader(GLenum shaderType) {
    return glCreateShader(shaderType);
}

void OpenGL::depthFunc(GLenum func) {
    glDepthFunc(func);
}

void OpenGL::depthMask(GLboolean flag) {
    glDepthMask(flag);
}

void OpenGL::disable(GLenum mode) {
    glDisable(mode);
}

void OpenGL::drawBuffer(GLenum mode) {
    glDrawBuffer(mode);
}

void OpenGL::enable(GLenum mode) {
    glEnable(mode);
}

void OpenGL::enableClientState(GLenum cap) {
    glEnableClientState(cap);
}

void OpenGL::getActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void OpenGL::getFloatv(GLenum pname, GLfloat * params) {
    glGetFloatv(pname, params);
}

void OpenGL::getIntegerv(GLenum pname, GLint * params) {
    glGetIntegerv(pname, params);
}

void OpenGL::getShaderiv(GLuint shader, GLenum pname, GLint * ptr) {
    glGetShaderiv(shader, pname, ptr);
}

void OpenGL::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog) {
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

const char * OpenGL::getString(GLenum name) {
    return (const char *)glGetString(name);
}

GLint OpenGL::getUniformLocation(GLuint program, const char * name) {
    return glGetUniformLocation(program, name);
}

void OpenGL::linkProgram(GLuint program) {
    glLinkProgram(program);
}

void OpenGL::loadMatrix(GLfloat * matrix) {
    glLoadMatrixf(matrix);
}

void OpenGL::matrixMode(GLenum mode) {
    glMatrixMode(mode);
}

void OpenGL::normalPointer(GLenum type, GLsizei stride, GLvoid * pointer) {
    glNormalPointer(type, stride, pointer);
}

void OpenGL::readBuffer(GLenum mode) {
    glReadBuffer(mode);
}

void OpenGL::secondaryColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glSecondaryColorPointer(size, type, stride, pointer);
}

void OpenGL::shaderSource(GLuint id, GLsizei count, const GLchar ** strings, const GLint * sizes) {
    glShaderSource(id, count, strings, sizes);
}

void OpenGL::texCoordPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glTexCoordPointer(size, type, stride, pointer);
}

void OpenGL::useProgram(GLuint program) {
    glUseProgram(program);
}

void OpenGL::vertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glVertexPointer(size, type, stride, pointer);
}

void OpenGL::viewport(GLint x, GLint y, GLint w, GLint h) {
    glViewport(x,y,w,h);
}

