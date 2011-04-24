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

void GraphicAPI::alphaFunc(GLenum func, GLclampf ref) {
    glAlphaFunc(func, ref);
}

void GraphicAPI::attachShader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
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

void GraphicAPI::enable(GLenum mode) {
    glEnable(mode);
}

void GraphicAPI::getActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void GraphicAPI::getShaderiv(GLuint shader, GLenum pname, GLint * ptr) {
    glGetShaderiv(shader, pname, ptr);
}

void GraphicAPI::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog) {
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

GLint GraphicAPI::getUniformLocation(GLuint program, const char * name) {
    return glGetUniformLocation(program, name);
}

void GraphicAPI::linkProgram(GLuint program) {
    glLinkProgram(program);
}

void GraphicAPI::shaderSource(GLuint id, GLsizei count, const GLchar ** strings, const GLint * sizes) {
    glShaderSource(id, count, strings, sizes);
}

void GraphicAPI::useProgram(GLuint program) {
    glUseProgram(program);
}

void GraphicAPI::viewport(GLint x, GLint y, GLint w, GLint h) {
    glViewport(x,y,w,h);
}

