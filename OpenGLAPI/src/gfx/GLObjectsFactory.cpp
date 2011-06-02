#include <GL/glew.h>
#include <vector>

#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Shader.h"
#include "OpenGLAPI/gfx/GLObjectsFactory.h"
#include "OpenGLAPI/gfx/GLBuffer.h"
#include "OpenGLAPI/gfx/GLTextures.h"


using namespace pbge;

GLObjectsFactory::GLObjectsFactory(GraphicAPI * ogl) {
    this->gl = ogl;
}

Buffer * GLObjectsFactory::createBuffer(size_t _size, Buffer::UsageHint _usage) {
    return new GLBuffer(_size, _usage, gl);
}

Texture1D * GLObjectsFactory::create1DTexture() {
    return new GLTexture1D(gl);
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

GPUProgram * GLObjectsFactory::createProgram(const std::vector<Shader*> & vertexShaders, const std::vector<Shader *> & fragShaders) {
    GLProgram * program = new GLProgram();
    std::vector<Shader *>::const_iterator it;
    for(it = vertexShaders.begin(); it != vertexShaders.end(); it++) {
        GLShader * shader = dynamic_cast<GLShader*>(*it);
        if(shader == NULL) {
            throw IllegalArgumentException("invalid shader class type");
        }
        if(shader->getType() != Shader::VERTEX_SHADER) {
            throw IllegalArgumentException("invalid shader type");
        }
        program->attachShader(shader);
    }
    for(it = fragShaders.begin(); it != fragShaders.end(); it++) {
        GLShader * shader = dynamic_cast<GLShader*>(*it);
        if(shader == NULL) {
            throw IllegalArgumentException("invalid shader class type");
        }
        if(shader->getType() != Shader::FRAGMENT_SHADER) {
            throw IllegalArgumentException("invalid shader type");
        }
        program->attachShader(shader);
    }
    return program;
}

GPUProgram * GLObjectsFactory::createProgramFromString(const std::string & vsSource, const std::string & fsSource) {
    return GLProgram::fromString(vsSource, fsSource);
}
