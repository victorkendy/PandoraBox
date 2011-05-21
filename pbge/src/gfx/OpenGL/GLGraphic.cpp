#include <string>

#include "pbge/gfx/VBO.h"
#include "pbge/gfx/OpenGL/GLGraphic.h"
#include "pbge/gfx/OpenGL/GLDrawController.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/OpenGL/GLObjectsFactory.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

GLGraphic::GLGraphic() {
    matrices = new math3d::matrix44[3];
    matrices[2] = math3d::identity44;
    this->state = NULL;
    storage = new ResourceStorage;
    this->context = NULL;
    this->factory = new GLObjectsFactory(this);
    this->projectionUpdated = true;
    this->drawController = new GLDrawController(this);
    createDefaultShaders();
}

GLGraphic::~GLGraphic() {
    delete [] matrices;
    delete state;
    delete context;
}

void GLGraphic::createDefaultShaders() {
    std::string defaultVertexShader = 
        "void calculateVertex(inout vec4 vertex, inout vec3 normal, inout vec4 color);\n"
        "varying vec4 position;\n"
        "varying vec3 normal;\n"
        "void main() {\n"
        "   vec4 vertex = gl_Vertex;\n"
        "   vec3 _normal = gl_Normal;\n"
        "   vec4 color = gl_Color;\n"
        "   calculateVertex(vertex, _normal, color);\n"
        "   gl_Position = gl_ProjectionMatrix * vertex; gl_FrontColor = color;\n"
        "   position = vertex;\n"
        "   normal = _normal;"
        "}";
    std::string defaultRenderShader = 
        "void calculateFragmentColor(inout vec4 color);\n"
        "void main() {\n"
        "   vec4 color = gl_Color;\n"
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
    this->context = newContext;
    if(context != NULL) {
        context->makeCurrent();
        glewInit();
        GLint initialMatrixMode;
        glGetIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
        currentMatrixMode = initialMatrixMode;
        state = new StateSet(this);
        drawController->initialize();
    }
}

GraphicContext * GLGraphic::getContext() {
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
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void GLGraphic::enableDrawBuffer(GLenum buffer) {
    glDrawBuffer(buffer);
    glReadBuffer(buffer);
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
    return numberOfUnits;
}

void GLGraphic::pushUniforms(UniformSet * uniforms) {
    this->state->pushUniformSet(uniforms);
}

void GLGraphic::popUniforms() {
    this->state->popUniformSet();
}

DrawController * GLGraphic::getDrawController() {
    return drawController;
}

