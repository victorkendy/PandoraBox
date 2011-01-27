
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/Light.h"

namespace {
    const std::string pointLightVSSource = 
        "uniform vec4 aha;\n"
        "varying vec3 normal;\n"
        "varying vec4 position;\n"
        "void main() {\n"
        "   normal = gl_NormalMatrix * gl_Normal;\n"
        "   gl_FrontColor = gl_Color + aha;"
        "   gl_Position = ftransform();\n"
        "}\n";

    const std::string pointLightFSSource = 
        "varying vec3 normal;\n"
        "varying vec4 position;\n"
        "uniform vec4 light_position;"
        "void main() {\n"
        "   float ndotd;\n"
        "   vec4 lightPosition = light_position;\n"
        "   vec4 lightColor = vec4(1,1,1,1);\n"
        "   vec3 dirVector = lightPosition.xyz - position.xyz;\n"
        "   dirVector = normalize(dirVector);\n"
        "   ndotd = max(0.0, dot(normal, dirVector));\n"
        "   gl_FragColor = lightColor * ndotd * gl_Color;\n"
        "}\n";

    pbge::GLShader * defaultPointLightVS = NULL;
    pbge::GLShader * defaultPointLightFS = NULL;
    pbge::GLProgram * defaultPointLightProgram = NULL;
}

using namespace pbge;

void PointLight::updatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    if(isLightOn())
        visitor->addActiveLight(this);
    math3d::matrix44 m = visitor->getCurrentTransformation();
    (*position)[0] = m[0][3];
    (*position)[1] = m[1][3];
    (*position)[2] = m[2][3];
    (*position)[3] = m[3][3];
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

void PointLight::setNecessaryUniforms(OpenGL * ogl) {
    UniformFloatVec4 * position = dynamic_cast<UniformFloatVec4*>(ogl->getState().getUniformValue(UniformInfo("light_position", GL_FLOAT_VEC4, -1)));
    math3d::vector4 p = this->getPosition();
    position->setValue(p[0],p[1],p[2],p[3]);
}
