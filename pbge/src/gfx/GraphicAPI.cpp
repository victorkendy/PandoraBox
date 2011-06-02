

#include <GL/glew.h>
#include <iostream>

#include "pbge/core/ExternalFactoryLoader.h"
#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"



using namespace pbge;
ExternalFactoryLoader<GraphicAPI> lib("OpenGLAPI.dll", "createInstance");

GraphicAPI * GraphicAPI::createInstance() {
    GraphicAPI * api = lib.instantiate();
    return api;
}

void GraphicAPI::alphaFunc(GLenum func, GLclampf ref) {
    glAlphaFunc(func, ref);
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

void GraphicAPI::clientActiveTexture(GLenum textureUnit) {
    glClientActiveTexture(textureUnit);
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

