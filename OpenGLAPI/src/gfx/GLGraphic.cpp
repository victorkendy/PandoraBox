#include <GL/glew.h>
#include <string>
#include <iostream>

#include "pbge/gfx/VBO.h"
#include "OpenGLAPI/gfx/GLGraphic.h"
#include "OpenGLAPI/gfx/GLDrawController.h"
#include "OpenGLAPI/gfx/GLDepthBufferController.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/FramebufferObject.h"
#include "OpenGLAPI/gfx/GLObjectsFactory.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

GLGraphic::GLGraphic():matrices(new math3d::matrix44[3]), state(NULL), storage(new ResourceStorage), context(NULL), majorVersion(0){
    matrices[2] = math3d::identity44;
    factory.reset(new GLObjectsFactory(this));
    drawController.reset(new GLDrawController(this));
}

void GLGraphic::setContext(GraphicContext * newContext) {
    context.reset(newContext);
    if(context != NULL) {
        context->makeCurrent();
        glewInit();
        state.reset(new StateSet(this));
        depthController.reset(new GLDepthBufferController);
        blendController.reset(new GLBlendController);
        initContextVersion();
        drawController->initialize();
        extensions.reset(new GLExtensions(this));
    }
}

void GLGraphic::initContextVersion() {
    // Parse the version string?
    this->majorVersion = 1;
    if(GLEW_VERSION_2_0 || GLEW_VERSION_2_1) {
        this->majorVersion = 2;
    }
    if(GLEW_VERSION_3_0 || GLEW_VERSION_3_1 || GLEW_VERSION_3_2 || GLEW_VERSION_3_3) {
        this->majorVersion = 3;
    }
    if(GLEW_VERSION_4_0) {
        this->majorVersion = 4;
    }
    std::cout << "Detected version: " << majorVersion << std::endl;
}

GraphicContext * GLGraphic::getContext() {
    return context.get();
}

void GLGraphic::releaseContext() {
    if(context != NULL) {
        context->release();
        context.reset(NULL);
    }
}

void GLGraphic::makeContextCurrent() {
    context->makeCurrent();
}

void GLGraphic::swapBuffers() {
    context->swapBuffers();
}

void GLGraphic::loadViewMatrix(const math3d::matrix44 & m) {
    matrices[0] = m;
}

void GLGraphic::loadProjectionMatrix(const math3d::matrix44 & m) {
    matrices[1] = m;
}

void GLGraphic::loadModelMatrix(const math3d::matrix44 & m) {
    matrices[2] = m;
}

void GLGraphic::updateState() {
    this->state->apply(this);
}

GraphicObjectsFactory * GLGraphic::getFactory() {
    return factory.get();
}

UniformValue * GLGraphic::getUniformValue(const UniformInfo & info) {
    return this->state->getUniformValue(info);
}

UniformValue * GLGraphic::searchUniform(const UniformInfo & info) {
    return this->state->searchUniform(info);
}

void GLGraphic::enableMode(Mode mode) {
    this->state->enable(mode);
}

void GLGraphic::disableDrawBuffer() {
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void GLGraphic::enableDrawBuffer(GLenum buffer) {
    glDrawBuffer(buffer);
    glReadBuffer(buffer);
}

StateSet * GLGraphic::getState() { 
    return state.get();
}

ResourceStorage * GLGraphic::getStorage() { 
    return storage.get();
}

TextureUnit * GLGraphic::chooseTextureUnit(Texture * texture) {
    return state->chooseTexUnit(texture);
}

const int GLGraphic::numberOfTextureUnits() {
    GLint numberOfUnits = -1;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numberOfUnits);
    return numberOfUnits;
}

void GLGraphic::pushUniforms(UniformSet * uniforms) {
    this->state->pushUniformSet(uniforms);
}

void GLGraphic::popUniforms() {
    this->state->popUniformSet();
}

DrawController * GLGraphic::getDrawController() {
    return drawController.get();
}

void GLGraphic::setViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void GLGraphic::bindFramebufferObject(FramebufferObject * fbo) {
    state->useFBO(fbo);
}

GPUProgram * GLGraphic::getCurrentProgram() {
    return this->state->currentProgram();
}

DepthBufferController * GLGraphic::depthBufferController() {
    return depthController.get();
}

BlendController * GLGraphic::getBlendController() {
    return blendController.get();
}