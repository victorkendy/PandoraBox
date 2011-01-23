#ifndef PBGE_GFX_SHADER_H_
#define PBGE_GFX_SHADER_H_ 1

#include <string>
#include <vector>

#include "pbge/gfx/OpenGL.h"
#include "pbge/core/core.h"

namespace pbge {
    class FileReader;

    class PBGE_EXPORT Shader {
    public:
        typedef enum {
            VERTEX_SHADER,
            FRAGMENT_SHADER
        } ShaderType;

        virtual bool compile(OpenGL * ogl) = 0;
    };

    class PBGE_EXPORT GPUProgram {
    public:
        virtual bool bind(OpenGL * ogl) = 0;

        virtual void unbind(OpenGL * ogl) = 0;

        virtual bool link(OpenGL * ogl) = 0;

        virtual bool isLinked() = 0;

        virtual const std::string getInfoLog() = 0;

        virtual GLuint getId() = 0;
    };


    class PBGE_EXPORT GLShader : public Shader{
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

        bool compile(OpenGL * ogl);

        GLuint getID() { 
            return shaderID; 
        }

    private:
        GLuint shaderID;

        ShaderType type;

        char * source;

        bool compiled;
    };

    class PBGE_EXPORT GLProgram : public GPUProgram{
    public:
        GLProgram() {
            programID = 0;
            linked = false;
        }

        bool bind(OpenGL * ogl);

        void unbind(OpenGL * ogl);

        void attachShader(GLShader *shader);

        bool link(OpenGL * ogl);

        const std::string getInfoLog() {
            return infoLog;
        }

        bool isLinked() {
            return linked;
        }

        static GLProgram * fromString(const std::string &vertexShader, const std::string &fragmentShader);

        static GLProgram * fromFile(FileReader * vertexShaderFile, FileReader *  fragmentShaderFile);

        GLuint getId() { return programID; }
    private:
        void extractInfoLog(OpenGL * ogl);

        bool linked;

        GLuint programID;

        std::vector<GLShader*> attachedShaders;

        std::string infoLog;
    };
}

#endif
