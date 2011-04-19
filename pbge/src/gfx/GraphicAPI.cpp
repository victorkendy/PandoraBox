#include <GL/glew.h>
#include <iostream>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/OpenGL/GLGraphic.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/OpenGL/GLBuffer.h"



using namespace pbge;

GraphicAPI * GraphicAPI::createInstance() {
    return new GLGraphic;
}

//
//void GraphicAPI::setMatrixMode(GLenum mode) {
//    if(currentMatrixMode != mode)
//        glMatrixMode(mode);
//    currentMatrixMode = mode;
//}
//
//void GraphicAPI::uploadModelview() {
//    glMatrixMode(GL_MODELVIEW);
//    glLoadMatrixf((matrices[0]*matrices[2]).transpose());
//}
//
//void GraphicAPI::uploadProjection() {
//    glMatrixMode(GL_PROJECTION);
//    glLoadMatrixf(matrices[1].transpose());
//}
//
//void GraphicAPI::updateState() {
//    this->uploadModelview();
//    this->state->apply(this);
//}
//
//GraphicObjectsFactory * GraphicAPI::getFactory() {
//    return this->factory;
//}
//
//UniformValue * GraphicAPI::getUniformValue(const UniformInfo & info) {
//    return this->state->getUniformValue(info);
//}
//
//UniformValue * GraphicAPI::searchUniform(const UniformInfo & info) {
//    return this->state->searchUniform(info);
//}
//
//void GraphicAPI::enableMode(Mode mode) {
//    this->state->enable(mode);
//}
//
//void GraphicAPI::disableDrawBuffer() {
//    this->drawBuffer(GL_NONE);
//    this->readBuffer(GL_NONE);
//}
//
//void GraphicAPI::enableDrawBuffer(GLenum buffer) {
//    this->drawBuffer(buffer);
//    this->readBuffer(buffer);
//}
//
//TextureUnit * GraphicAPI::chooseTextureUnit(Texture * texture) {
//    return state->chooseTexUnit(texture);
//}
//
//const int GraphicAPI::numberOfTextureUnits() {
//    GLint numberOfUnits = -1;
//    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numberOfUnits);
//    std::cout << "number of texture units: " << numberOfUnits << std::endl;
//    return numberOfUnits;
//}
//
//void GraphicAPI::pushUniforms(UniformSet * uniforms) {
//    this->state->pushUniformSet(uniforms);
//}
//
//void GraphicAPI::popUniforms() {
//    this->state->popUniformSet();
//}


void GraphicAPI::alphaFunc(GLenum func, GLclampf ref) {
    glAlphaFunc(func, ref);
}

void GraphicAPI::attachShader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
}

void GraphicAPI::begin(GLenum mode) {
    glBegin(mode);
}

void GraphicAPI::bindAttribLocation(GLuint program, GLuint index, const GLchar* name) {
    glBindAttribLocation(program, index, name);
}

void GraphicAPI::blendFunc(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void GraphicAPI::clear(GLbitfield mask) {
    glClear(mask);
}

void GraphicAPI::clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    glClearColor(red, green, blue, alpha);
}

void GraphicAPI::clearDepth(GLclampd depth) {
    glClearDepth(depth);
}

void GraphicAPI::clientActiveTexture(GLenum textureUnit) {
    glClientActiveTexture(textureUnit);
}

void GraphicAPI::colorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glColorPointer(size, type, stride, pointer);
}

void GraphicAPI::compileShader(GLuint shader) {
    glCompileShader(shader);
}

GLuint GraphicAPI::createProgram(){
    return glCreateProgram();
}

GLuint GraphicAPI::createShader(GLenum shaderType) {
    return glCreateShader(shaderType);
}

void GraphicAPI::depthFunc(GLenum func) {
    glDepthFunc(func);
}

void GraphicAPI::depthMask(GLboolean flag) {
    glDepthMask(flag);
}

void GraphicAPI::disable(GLenum mode) {
    glDisable(mode);
}

void GraphicAPI::drawBuffer(GLenum mode) {
    glDrawBuffer(mode);
}

void GraphicAPI::enable(GLenum mode) {
    glEnable(mode);
}

void GraphicAPI::enableClientState(GLenum cap) {
    glEnableClientState(cap);
}

void GraphicAPI::getActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void GraphicAPI::getFloatv(GLenum pname, GLfloat * params) {
    glGetFloatv(pname, params);
}

void GraphicAPI::getIntegerv(GLenum pname, GLint * params) {
    glGetIntegerv(pname, params);
}

void GraphicAPI::getShaderiv(GLuint shader, GLenum pname, GLint * ptr) {
    glGetShaderiv(shader, pname, ptr);
}

void GraphicAPI::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog) {
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

const char * GraphicAPI::getString(GLenum name) {
    return (const char *)glGetString(name);
}

GLint GraphicAPI::getUniformLocation(GLuint program, const char * name) {
    return glGetUniformLocation(program, name);
}

void GraphicAPI::linkProgram(GLuint program) {
    glLinkProgram(program);
}

void GraphicAPI::loadMatrix(GLfloat * matrix) {
    glLoadMatrixf(matrix);
}

void GraphicAPI::matrixMode(GLenum mode) {
    glMatrixMode(mode);
}

void GraphicAPI::normalPointer(GLenum type, GLsizei stride, GLvoid * pointer) {
    glNormalPointer(type, stride, pointer);
}

void GraphicAPI::readBuffer(GLenum mode) {
    glReadBuffer(mode);
}

void GraphicAPI::secondaryColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glSecondaryColorPointer(size, type, stride, pointer);
}

void GraphicAPI::shaderSource(GLuint id, GLsizei count, const GLchar ** strings, const GLint * sizes) {
    glShaderSource(id, count, strings, sizes);
}

void GraphicAPI::texCoordPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glTexCoordPointer(size, type, stride, pointer);
}

void GraphicAPI::useProgram(GLuint program) {
    glUseProgram(program);
}

void GraphicAPI::vertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer) {
    glVertexPointer(size, type, stride, pointer);
}

void GraphicAPI::viewport(GLint x, GLint y, GLint w, GLint h) {
    glViewport(x,y,w,h);
}

