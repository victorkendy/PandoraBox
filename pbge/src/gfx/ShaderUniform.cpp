
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/ShaderUniform.h"

using namespace pbge;

void UniformFloatVec4::bindValueOn(GPUProgram *program, const UniformInfo & info, OpenGL *ogl) {
    program->bindFloatVec4(info, ogl, values[0], values[1], values[2], values[3]);
}

void UniformFloatVec3::bindValueOn(GPUProgram *program, const UniformInfo & info, OpenGL *ogl) {
    program->bindFloatVec3(info, ogl, values[0], values[1], values[2]);
}

void UniformFloatVec2::bindValueOn(GPUProgram *program, const UniformInfo & info, OpenGL *ogl) {
    program->bindFloatVec2(info, ogl, values[0], values[1]);
}

void UniformFloat::bindValueOn(GPUProgram *program, const UniformInfo & info, OpenGL *ogl) {
    program->bindFloat(info, ogl, value);
}

void UniformSampler2D::bindValueOn(GPUProgram *program, const UniformInfo &info, OpenGL *ogl) {
    program->bindSampler2D(info, ogl, texture);
}