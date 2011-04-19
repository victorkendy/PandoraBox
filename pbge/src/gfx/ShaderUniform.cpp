
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/ShaderUniform.h"

using namespace pbge;

void UniformFloatVec4::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    program->bindFloatVec4(info, ogl, values[0], values[1], values[2], values[3]);
}

void UniformFloatVec3::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    program->bindFloatVec3(info, ogl, values[0], values[1], values[2]);
}

void UniformFloatVec2::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    program->bindFloatVec2(info, ogl, values[0], values[1]);
}

void UniformFloat::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    program->bindFloat(info, ogl, value);
}

void UniformSampler2D::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindSampler2D(info, ogl, texture);
}

void UniformMat4::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindMat4(info, ogl, this->values);
}