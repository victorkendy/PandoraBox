#include "pbge/gfx/OpenGL/GLGraphic.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/OpenGL/GLObjectsFactory.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

GLGraphic::GLGraphic() {
    std::cout << "instancing GLGraphic" << std::endl;
    matrices = new math3d::matrix44[3];
    matrices[2] = math3d::identity44;
    this->state = NULL;
    storage = new ResourceStorage;
    this->context = NULL;
    this->factory = new GLObjectsFactory(this);
}

GLGraphic::~GLGraphic() {
    delete [] matrices;
    delete state;
    delete context;
}

void GLGraphic::setContext(GLContext * newContext) {
    this->context = newContext;
    if(context != NULL) {
        context->makeCurrent();
        glewInit();
        GLint initialMatrixMode;
        this->getIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
        currentMatrixMode = initialMatrixMode;
        state = new StateSet(this);
    }
}

GLContext * GLGraphic::getContext() {
    return context;
}

void GLGraphic::releaseContext() {
    if(context != NULL) {
        context->release();
        context = NULL;
    }
}

void GLGraphic::makeContextCurrent() {
    context->makeCurrent();
}

void GLGraphic::swapBuffers() {
    context->swapBuffers();
}

void GLGraphic::setMatrixMode(GLenum mode) {
    if(currentMatrixMode != mode)
        glMatrixMode(mode);
    currentMatrixMode = mode;
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
    this->uploadModelview();
    this->state->apply(this);
}

void GLGraphic::uploadModelview() {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((matrices[0]*matrices[2]).transpose());
}

void GLGraphic::uploadProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrices[1].transpose());
}

GraphicObjectsFactory * GLGraphic::getFactory() {
    return this->factory;
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
    this->drawBuffer(GL_NONE);
    this->readBuffer(GL_NONE);
}

void GLGraphic::enableDrawBuffer(GLenum buffer) {
    this->drawBuffer(buffer);
    this->readBuffer(buffer);
}

StateSet * GLGraphic::getState() { 
    return state; 
}

ResourceStorage * GLGraphic::getStorage() { 
    return storage; 
}

TextureUnit * GLGraphic::chooseTextureUnit(Texture * texture) {
    return state->chooseTexUnit(texture);
}

const int GLGraphic::numberOfTextureUnits() {
    GLint numberOfUnits = -1;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numberOfUnits);
    std::cout << "number of texture units: " << numberOfUnits << std::endl;
    return numberOfUnits;
}

void GLGraphic::pushUniforms(UniformSet * uniforms) {
    this->state->pushUniformSet(uniforms);
}

void GLGraphic::popUniforms() {
    this->state->popUniformSet();
}