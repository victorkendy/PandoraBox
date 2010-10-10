#ifndef gfxshader_h_
#define gfxshader_h_ 1

#include <string>
#include "math3d/math3d.h"
#include "pbge/core/core.h"
#include "pbge/core/definitions.h"
#include "pbge/core/Object.h"

namespace pbge {

    class PBGE_EXPORT Shader : public Object{
    public:
        virtual int loadSourceFromFile(std::string fileName, const int type) = 0;
        virtual int loadSource(std::string source, const int type) = 0;
    };

    class PBGE_EXPORT GPUProgram : public Object{
    public:
        GPUProgram() { _isCompiled = PBGE_false; }
        virtual int bind() = 0;
        virtual void unbind() = 0;
        // É esperado que o shader seja compilado antes de chamar bind()
        virtual int compile() = 0;

        virtual GLuint getId() = 0;
    protected:
        int _isCompiled;
    };
    /*
    class GLUniform : public Object {
        class UniformType {};
    public:
        virtual void bind(GLuint programID)=0;
    protected:
        GLuint location;
        std::string name;
    };

    class GLFloatUniform:public GLUniform {
    public:
        void bind(GLuint programID);
    protected:
        float * data;
        int size;
    };
    */
    class PBGE_EXPORT GLShader : public Shader{
    public:
        GLShader() { shaderID = 0; }
        int loadSourceFromFile(std::string fileName, const int type);
        int loadSource(std::string source, const int type);
        GLuint getID() { return shaderID; }
    protected:
        int shaderFromPointer(const char *ptr, const int type);
        GLuint shaderID;
    };

    class PBGE_EXPORT GLProgram : public GPUProgram{
    public:
        int bind();
        void unbind();
        int attachShader(GLShader &shader);
        int attachShader(GLShader *shader);
        int compile();
        int compileFromString(const std::string &vertexShader, const std::string &fragmentShader);
        int compileFromFile(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
        
        //void sendVector4(const math3d::vector4 & v, const std::string & varName);

        GLuint getId() { return programID; }
    protected:
        GLuint programID;
    };
}

#endif
