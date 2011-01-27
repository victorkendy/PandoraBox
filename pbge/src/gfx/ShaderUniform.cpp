
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/ShaderUniform.h"

using namespace pbge;

void UniformFloatVec4::bindValueOn(GPUProgram *program, const UniformInfo & info, OpenGL *ogl) {
    program->bindFloatVec4(info, ogl, values[0], values[1], values[2], values[3]);
}