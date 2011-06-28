#include <GL/glew.h>
#include <string>

#include "pbge/gfx/VBO.h"
#include "OpenGLAPI/gfx/GLGraphic.h"
#include "OpenGLAPI/gfx/GLDrawController.h"
#include "pbge/gfx/ResourceStorage.h"
#include "OpenGLAPI/gfx/GLObjectsFactory.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

GLGraphic::GLGraphic():matrices(new math3d::matrix44[3]), state(NULL), storage(new ResourceStorage), context(NULL), projectionUpdated(true), majorVersion(0) {
    matrices[2] = math3d::identity44;
    factory.reset(new GLObjectsFactory(this));
    drawController.reset(new GLDrawController(this));
    createDefaultShaders();
}

GLGraphic::~GLGraphic() {
}

void GLGraphic::createDefaultShaders() {
    std::string defaultVertexShader = 
        "void calculateVertex(out vec4 vertex, out vec3 normal, out vec4 color);\n"
        "varying vec4 position;\n"
        "varying vec3 normal;\n"
        "void main() {\n"
        "   vec4 vertex;\n"
        "   vec3 _normal;\n"
        "   vec4 color;\n"
        "   calculateVertex(vertex, _normal, color);\n"
        "   gl_Position = gl_ProjectionMatrix * vertex; gl_FrontColor = color;\n"
        "   position = vertex;\n"
        "   normal = _normal;"
        "}";
    std::string defaultRenderShader = 
        "void calculateFragmentColor(out vec4 color);\n"
        "void main() {\n"
        "   vec4 color;\n"
        "   calculateFragmentColor(color);\n"
        "   gl_FragColor = color;\n"
        "}";
    std::string defaultDepthShader =
        "varying vec4 position;\n"
        "void calculateDepth(in vec4 normalizedPosition, out float depth);\n"
        "void main(){\n"
        "   float depth;\n"
        "   gl_FragDepth = gl_FragCoord.z;\n"
        "}";
    Shader * vertexShader = getFactory()->createShaderFromString(defaultVertexShader, Shader::VERTEX_SHADER);
    Shader * renderShader = getFactory()->createShaderFromString(defaultRenderShader, Shader::FRAGMENT_SHADER);
    Shader * depthShader = getFactory()->createShaderFromString(defaultDepthShader, Shader::FRAGMENT_SHADER);
    
    getStorage()->storeNamedShader("pbge.defaultMainVertexShader", vertexShader);
    getStorage()->storeNamedShader("pbge.defaultMainRenderPassShader", renderShader);
    getStorage()->storeNamedShader("pbge.defaultMainDepthPassShader", depthShader);
}

void GLGraphic::setContext(GraphicContext * newContext) {
    context.reset(newContext);
    if(context != NULL) {
        context->makeCurrent();
        glewInit();
        GLint initialMatrixMode;
        glGetIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
        currentMatrixMode = initialMatrixMode;
        state.reset(new StateSet(this));
        initContextVersion();
        drawController->initialize();
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

void GLGraphic::setMatrixMode(GLenum mode) {
    if(currentMatrixMode != mode)
        glMatrixMode(mode);
    currentMatrixMode = mode;
}

void GLGraphic::loadViewMatrix(const math3d::matrix44 & m) {
    matrices[0] = m;
}

void GLGraphic::loadProjectionMatrix(const math3d::matrix44 & m) {
    projectionUpdated = true;
    matrices[1] = m;
}

void GLGraphic::loadModelMatrix(const math3d::matrix44 & m) {
    matrices[2] = m;
}

void GLGraphic::updateState() {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((matrices[0]*matrices[2]).transpose());
    if(projectionUpdated) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matrices[1].transpose());
    }
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