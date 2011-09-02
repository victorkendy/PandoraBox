#include <algorithm>

#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/ShaderUniform.h"

using namespace pbge;

void UniformFloatVec4::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    float * values = static_cast<float *>(getValueAt(0));
    program->bindFloatVec4(info, ogl, values, std::min(getNumberOfElements(), info.getNumberOfElements()));
}

void UniformFloatVec3::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    float * values = static_cast<float *>(getValueAt(0));
    program->bindFloatVec3(info, ogl, values, std::min(getNumberOfElements(), info.getNumberOfElements()));
}

void UniformFloatVec2::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    float * values = static_cast<float *>(getValueAt(0));
    program->bindFloatVec2(info, ogl, values, std::min(getNumberOfElements(), info.getNumberOfElements()));
}

void UniformFloat::bindValueOn(GPUProgram *program, const UniformInfo & info, GraphicAPI *ogl) {
    float * values = static_cast<float *>(getValueAt(0));
    program->bindFloat(info, ogl, values, std::min(getNumberOfElements(), info.getNumberOfElements()));
}

void UniformBufferSampler::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindBufferSampler(info, ogl, texture);
}

void UniformSampler1D::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindSampler1D(info, ogl, texture);
}

void UniformSampler2D::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindSampler2D(info, ogl, texture);
}

void UniformMat4::bindValueOn(GPUProgram *program, const UniformInfo &info, GraphicAPI *ogl) {
    program->bindMat4(info, ogl, this->values);
}