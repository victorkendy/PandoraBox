#ifndef PBGE_GFX_GRAPHICOBJECTSFACTORY_H
#define PBGE_GFX_GRAPHICOBJECTSFACTORY_H

#include <vector>
#include <string>

#include "pbge/core/core.h"
#include "pbge/gfx/Shader.h"


namespace pbge {
    class Buffer;
    class Texture2D;
    class FileReader;

    class GraphicObjectsFactory {
    public:
        /** Creates a buffer object with the specified size.

            @param size The size of the buffer object storage
            @param usage The expected buffer usage
            @return The created buffer object
        */
        virtual Buffer * createBuffer(size_t size, Buffer::UsageHint usage) = 0;

        /** Creates a API specific 2D texture object.

            @return A new 2D texture object
        */
        virtual Texture2D * create2DTexture() = 0;

        /** Creates a new shader with the source code from a specified file.

            @param file The file with the source code for the new shader
            @param type The type of the shader object to be created
            @return A new shader object with the source specified by the file content
        */
        virtual Shader * createShaderFromFile(FileReader * file, Shader::ShaderType type) = 0;
        
        /** Creates a new shader with the given source.
            
            @param source The source code for the shader
            @param type The type for the created shader
            @return A new shader object
        */
        virtual Shader * createShaderFromString(const std::string & source, Shader::ShaderType type) = 0;

        /** Creates a empty program object.
            
            @return A new empty program instance
        */
        virtual GPUProgram * createProgram() = 0;

        /** Creates a new program object from the shader sources.
            
            @param vsSource The file with the source code for the vertex shader
            @param fsShader The file with the source code for the fragment shader
            @return A new program object with the created shader objects
        */
        virtual GPUProgram * createProgramFromFile(FileReader * vsSource, FileReader * fsSource) = 0;

        /** Creates a new program object from the shader vectors.

            @param vertexShaders The vertex shaders to be attached to the created program
            @param fragShaders The fragment shaders to be attached to the created program
            @return A new program object with the shaders attached
        */
        virtual GPUProgram * createProgram(const std::vector<Shader*> & vertexShaders, const std::vector<Shader *> & fragShaders) = 0;

        /** Creates a new program object from the shader sources.
            
            @param vsSource The source code for the vertex shader
            @param fsShader The source code for the fragment shader
            @return A new program object with the created shader objects
        */
        virtual GPUProgram * createProgramFromString(const std::string & vsSource, const std::string & fsSource) = 0;
    };

}
#endif