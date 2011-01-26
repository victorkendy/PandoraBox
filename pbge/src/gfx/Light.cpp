
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/Light.h"

namespace {
    std::string pointLightVSSource;
    std::string pointLightFSSource;

    pbge::GLShader * defaultPointLightVS = NULL;
    pbge::GLShader * defaultPointLightFS = NULL;
    pbge::GLProgram * defaultPointLightProgram = NULL;
}

using namespace pbge;

void PointLight::updatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    if(isLightOn())
        visitor->addActiveLight(this);
}

Shader * PointLight::getDefaultLightPassVS() {
    if(defaultPointLightVS == NULL)
        defaultPointLightVS = GLShader::loadSource(pointLightVSSource, Shader::VERTEX_SHADER);
    return defaultPointLightVS;
}

Shader * PointLight::getDefaultLightPassFS() {
    if(defaultPointLightFS == NULL)
        defaultPointLightFS = GLShader::loadSource(pointLightFSSource, Shader::FRAGMENT_SHADER);
    return defaultPointLightFS;
}

GPUProgram * PointLight::getDefaultLightPassProgram() {
    if(defaultPointLightProgram == NULL) {
        defaultPointLightProgram = new GLProgram;
        defaultPointLightProgram->attachShader(dynamic_cast<GLShader*>(PointLight::getDefaultLightPassVS()));
        defaultPointLightProgram->attachShader(dynamic_cast<GLShader*>(PointLight::getDefaultLightPassFS()));
    }
    return defaultPointLightProgram;
}
