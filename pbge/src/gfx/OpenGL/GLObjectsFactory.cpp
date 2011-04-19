
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/OpenGL/GLObjectsFactory.h"
#include "pbge/gfx/OpenGL/GLBuffer.h"
#include "pbge/gfx/OpenGL/GLTextures.h"


using namespace pbge;

GLObjectsFactory::GLObjectsFactory(OpenGL * ogl) {
    this->gl = ogl;
}

Buffer * GLObjectsFactory::createBuffer(size_t _size, Buffer::UsageHint _usage) {
    return new GLBuffer(_size, _usage, gl);
}

Texture2D * GLObjectsFactory::create2DTexture() { 
    return new GLTexture2D(gl);
}

Shader * GLObjectsFactory::createShaderFromFile(FileReader * file, Shader::ShaderType type) {
    return GLShader::loadSourceFromFile(file, type);
}

Shader * GLObjectsFactory::createShaderFromString(const std::string & source, Shader::ShaderType type) {
    return GLShader::loadSource(source, type);
}

GPUProgram * GLObjectsFactory::createProgram() {
    return new GLProgram();
}

GPUProgram * GLObjectsFactory::createProgramFromFile(FileReader * vsSource, FileReader * fsSource) {
    return GLProgram::fromFile(vsSource, fsSource);
}

GPUProgram * GLObjectsFactory::createProgramFromString(const std::string & vsSource, const std::string & fsSource) {
    return GLProgram::fromString(vsSource, fsSource);
}
