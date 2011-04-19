#include <iostream>
#include <cstdlib>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/Light.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/ResourceStorage.h"


using namespace pbge;

void PointLight::updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {
    if(isLightOn())
        visitor->addActiveLight(this);
    math3d::matrix44 m = visitor->getCurrentTransformation();
    (*position)[0] = m[0][3];
    (*position)[1] = m[1][3];
    (*position)[2] = m[2][3];
    (*position)[3] = m[3][3];
}

Shader * PointLight::getDefaultLightPassVS(GraphicAPI * ogl) {
    const std::string pointLightVSSource =
        "uniform vec4 light_position;\n"
        "varying vec3 normal;\n"
        "varying vec3 lightDir;\n"

        "void main() {\n"
        "   gl_Position = ftransform();\n"
        "   vec4 position = gl_ModelViewMatrix * gl_Vertex;\n"
        "   normal = gl_NormalMatrix * gl_Normal;\n"
        "   lightDir = (light_position - position).xyz;\n"
        "}\n";

    Shader * shader = ogl->getStorage()->getNamedShader("pbge.defaultPointLightVertexShader");
    if(shader == NULL) {
        shader = GLShader::loadSource(pointLightVSSource, Shader::VERTEX_SHADER);
        ogl->getStorage()->storeNamedShader("pbge.defaultPointLightVertexShader", shader);
    }
    return shader;
}

Shader * PointLight::getDefaultLightPassFS(GraphicAPI * ogl) {
    const std::string pointLightFSSource = 
        "uniform vec4 light_diffuseColor;\n"
        "varying vec3 normal;\n"
        "varying vec3 lightDir;\n"
        "uniform sampler2D texture;\n"

        "void main() {\n"
        "   float ndotd;\n"
        "   vec3 dirVector = normalize(lightDir);\n"
        "   vec4 lightColor = light_diffuseColor;\n"
        "   ndotd = max(0.0, dot(normalize(normal), dirVector));\n"
        "   gl_FragColor = lightColor * ndotd;\n"
        "}\n";

    Shader * shader = ogl->getStorage()->getNamedShader("pbge.defaultPointLightFragmentShader");
    if(shader == NULL) {
        shader = GLShader::loadSource(pointLightFSSource, Shader::FRAGMENT_SHADER);
        ogl->getStorage()->storeNamedShader("pbge.defaultPointLightFragmentShader", shader);
    }
    return shader;
}

GPUProgram * PointLight::getDefaultLightPassProgram(GraphicAPI * ogl) {
    GPUProgram * program = ogl->getStorage()->getNamedProgram("pbge.defaultPointLightProgram");
    if(program == NULL) {
        GLProgram * glProgram = new GLProgram;
        glProgram->attachShader(dynamic_cast<GLShader*>(PointLight::getDefaultLightPassVS(ogl)));
        glProgram->attachShader(dynamic_cast<GLShader*>(PointLight::getDefaultLightPassFS(ogl)));
        ogl->getStorage()->storeNamedProgram("pbge.defaultPointLightProgram", glProgram);
        program = glProgram;
    }
    return program;
}

#define W 2
#define H 2

PointLight::PointLight() {
    on = true;
    program = NULL;
    position = new math3d::vector4;
    this->setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void PointLight::setNecessaryUniforms(GraphicAPI * ogl, const math3d::matrix44 & viewTransform) {
    UniformFloatVec4 * position = dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("light_position", FLOAT_VEC4, -1)));
    math3d::vector4 p = viewTransform * (this->getPosition());
    position->setValue(p);
    UniformFloatVec4 * color = dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("light_diffuseColor", FLOAT_VEC4, -1)));
    color->setValue(diffuseColor[0], diffuseColor[1], diffuseColor[2], diffuseColor[3]);
}
