#ifndef PBGE_GFX_OPENGL_GLOBJECTSFACTORY_H
#define PBGE_GFX_OPENGL_GLOBJECTSFACTORY_H

#include <vector>
#include <iostream>
#include <cstddef>
#include "pbge/gfx/GraphicObjectsFactory.h"

namespace pbge {
    class GLGraphic;

    class GLObjectsFactory : public GraphicObjectsFactory {
    public:
        GLObjectsFactory(GLGraphic * gl);

        Buffer * createBuffer(size_t _size, Buffer::UsageHint _usage);
        
        Texture1D * create1DTexture();

        Texture2D * create2DTexture();

        FramebufferObject * createFramebuffer(size_t w, size_t h);

        Shader * createShaderFromFile(FileReader * file, Shader::ShaderType type);

        Shader * createShaderFromString(const std::string & source, Shader::ShaderType type);

        GPUProgram * createProgram();

        GPUProgram * createProgramFromFile(FileReader * vsSource, FileReader * fsSource);

        GPUProgram * createProgram(const std::vector<Shader*> & vertexShaders, const std::vector<Shader *> & fragShaders);

        GPUProgram * createProgramFromString(const std::string & vsSource, const std::string & fsSource);
    private:
        GLGraphic * gl;
    };
}

#endif