#ifndef PBGE_GFX_OPENGL_GLSHADER_H
#define PBGE_GFX_OPENGL_GLSHADER_H
#include <string>

#include "pbge/gfx/Shader.h"

namespace pbge {
    class FileReader;
    class GraphicAPI;

    class GLShader : public Shader{
    public:
        GLShader() { 
            shaderID = 0;
            source = NULL;
            compiled = false;
        }

        static GLShader * loadSourceFromFile(FileReader * file, const ShaderType type);

        static GLShader * loadSource(const std::string & source, const ShaderType type);

        bool isCompiled() {
            return compiled;
        }

        bool compile(GraphicAPI * ogl);

        const std::string & getInfoLog() {
            return infoLog;
        }

        const ShaderType getType() {
            return type;
        }

        unsigned getID() { 
            return shaderID; 
        }

    private:

        void extractInfolog(GraphicAPI * ogl);

        std::string infoLog;

        GLuint shaderID;

        ShaderType type;

        char * source;

        bool compiled;
    };
}

#endif