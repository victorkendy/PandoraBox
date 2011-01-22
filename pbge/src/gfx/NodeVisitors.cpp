#include <vector>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/NodeVisitors.h"


using namespace pbge;


void UpdaterVisitor::visit(Node * node, OpenGL * ogl) {
    node->updatePass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postUpdatePass(this, ogl);
}

void UpdaterVisitor::pushTransformation(const math3d::matrix44 & m) {
    transformationStack[++stackIndex] = m;
}

void UpdaterVisitor::popTransformation() {
    --stackIndex;
}

const math3d::matrix44 UpdaterVisitor::getCurrentTransformation() {
    return transformationStack[stackIndex];
}


void ColorPassVisitor::visit(Node *node, OpenGL *ogl) {
    node->renderPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postRenderPass(this, ogl);
}

void DepthPassVisitor::visit(Node *node, OpenGL *ogl) {
    node->depthPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postDepthPass(this, ogl);
}


namespace {
    Shader * defaultLightPassFS = NULL;
    Shader * defaultLightPassVS = NULL;
    GPUProgram * defaultLightPassProgram = NULL;

    const std::string defaultLightPassVSSource = 
        "varying vec3 normal;\n"
        "varying vec4 position;\n"
        "void main() {\n"
        "   normal = gl_NormalMatrix * gl_Normal;\n"
        "   gl_Position = ftransform();\n"
        "}\n";

    const std::string defaultLightPassFSSource = 
        "varying vec3 normal;\n"
        "varying vec4 position;\n"
        "void main() {\n"
        "   float ndotd;\n"
        "   vec3 lightPosition = vec3(0,2,1);\n"
        "   vec4 lightColor = vec4(1,1,1,1);\n"
        "   vec3 dirVector = lightPosition - position.xyz;\n"
        "   dirVector = normalize(dirVector);\n"
        "   ndotd = max(0.0, dot(normal, dirVector));\n"
        "   gl_FragColor = lightColor * ndotd;\n"
        //"   gl_FragColor = lightColor;\n"
        "}\n";
}

Shader * LightPassVisitor::getDefaultFragmentShader() {
    if(defaultLightPassFS == NULL)
        defaultLightPassFS = GLShader::loadSource(defaultLightPassFSSource, Shader::FRAGMENT_SHADER);
    return defaultLightPassFS;
}

Shader * LightPassVisitor::getDefaultVertexShader() {
    if(defaultLightPassVS == NULL) 
        defaultLightPassVS = GLShader::loadSource(defaultLightPassVSSource, Shader::VERTEX_SHADER);
    return defaultLightPassVS;
}

GPUProgram * LightPassVisitor::getDefaultProgram() {
    if(defaultLightPassProgram == NULL) {
        GLProgram * program = new GLProgram();
        program->attachShader(dynamic_cast<GLShader*>(LightPassVisitor::getDefaultVertexShader()));
        program->attachShader(dynamic_cast<GLShader*>(LightPassVisitor::getDefaultFragmentShader()));
        defaultLightPassProgram = program;
    }
    return defaultLightPassProgram;
}

void LightPassVisitor::visit(Node * node, pbge::OpenGL * ogl) {
    ogl->getState().useProgram(getLightPassProgram());
    node->renderPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postRenderPass(this, ogl);
}