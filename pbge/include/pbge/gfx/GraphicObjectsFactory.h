#ifndef PBGE_GFX_GRAPHICOBJECTSFACTORY_H
#define PBGE_GFX_GRAPHICOBJECTSFACTORY_H

#include <string>

#include "pbge/core/core.h"
#include "pbge/gfx/Shader.h"


namespace pbge {
    class Buffer;
    class Texture2D;
    class FileReader;

    class GraphicObjectsFactory {
    public:
        virtual Buffer * createBuffer(size_t _size, Buffer::UsageHint _usage) = 0;

        virtual Texture2D * create2DTexture() = 0;

        virtual Shader * createShaderFromFile(FileReader * file, Shader::ShaderType type) = 0;

        virtual Shader * createShaderFromString(const std::string & source, Shader::ShaderType type) = 0;

        virtual GPUProgram * createProgram() = 0;

        virtual GPUProgram * createProgramFromFile(FileReader * vsSource, FileReader * fsSource) = 0;

        virtual GPUProgram * createProgramFromString(const std::string & vsSource, const std::string & fsSource) = 0;
    };

}
#endif