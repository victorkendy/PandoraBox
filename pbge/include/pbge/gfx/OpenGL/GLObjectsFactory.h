#ifndef PBGE_GFX_OPENGL_GLOBJECTSFACTORY_H
#define PBGE_GFX_OPENGL_GLOBJECTSFACTORY_H

#include "pbge/gfx/GraphicObjectsFactory.h"

namespace pbge {
    class GraphicAPI;

    class GLObjectsFactory : public GraphicObjectsFactory {
    public:
        GLObjectsFactory(GraphicAPI * gl);

        Buffer * createBuffer(size_t _size, Buffer::UsageHint _usage);

        Texture2D * create2DTexture();

        Shader * createShaderFromFile(FileReader * file, Shader::ShaderType type);

        Shader * createShaderFromString(const std::string & source, Shader::ShaderType type);

        GPUProgram * createProgram();

        GPUProgram * createProgramFromFile(FileReader * vsSource, FileReader * fsSource);

        GPUProgram * createProgramFromString(const std::string & vsSource, const std::string & fsSource);
    private:
        GraphicAPI * gl;
    };
}

#endif