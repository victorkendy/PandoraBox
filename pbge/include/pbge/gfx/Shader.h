#ifndef PBGE_GFX_SHADER_H_
#define PBGE_GFX_SHADER_H_ 1

#include <string>
#include <vector>
#include <set>
#include <map>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/core/core.h"

namespace pbge {

    class FileReader;
    class Texture;
    class Texture1D;
    class Texture2D;
    class TextureBuffer;

    class PBGE_EXPORT Shader {
    public:
        typedef enum {
            VERTEX_SHADER,
            FRAGMENT_SHADER
        } ShaderType;
        
        virtual const std::string & getInfoLog()=0;

        virtual const ShaderType getType() = 0;

        virtual bool compile(GraphicAPI * ogl) = 0;
    };

    class PBGE_EXPORT GPUProgram {
    public:
        virtual void bind(GraphicAPI * ogl) = 0;

        virtual void unbind(GraphicAPI * ogl) = 0;

        virtual bool link(GraphicAPI * ogl) = 0;

        virtual bool isLinked() = 0;

        virtual const std::string getInfoLog() = 0;

        virtual GLuint getId() = 0;

        virtual void updateUniforms(GraphicAPI * ogl) = 0;

        virtual const std::vector<Shader*> getShaderOfType(Shader::ShaderType type) = 0;

        /** Return the index of the program output variable named name

            @param name The name of the shader output variable
            @return The 0 indexed location of the output variable or -1 if the output name does not exist
        */
        virtual int getOutputLocation(const std::string & name) = 0;

    public: // Uniform binding
        virtual void bindFloat(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) = 0;

        virtual void bindFloatVec2(const UniformInfo & info, GraphicAPI * ogl, const float* values, const unsigned size) = 0;

        virtual void bindFloatVec3(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) = 0;

        virtual void bindFloatVec4(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) = 0;

        virtual void bindBufferSampler(const UniformInfo & info, GraphicAPI * ogl, TextureBuffer * tex) = 0;

        virtual void bindSampler1D(const UniformInfo & info, GraphicAPI * ogl, Texture1D * tex) = 0;
        
        virtual void bindSampler2D(const UniformInfo & info, GraphicAPI * ogl, Texture2D * tex) = 0;

        virtual void bindMat4(const UniformInfo & info, GraphicAPI * ogl, const float * v) = 0;
    };

    

    // default shader implementation
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

        bool compile(GraphicAPI * ogl);

        const std::string & getInfoLog() {
            return infoLog;
        }

        const ShaderType getType() {
            return type;
        }

        GLuint getID() { 
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

    class PBGE_EXPORT GLProgram : public GPUProgram{
    public:
        GLProgram() {
            programID = 0;
            linked = false;
        }

        void bind(GraphicAPI * ogl);

        void unbind(GraphicAPI * ogl);

        void attachShader(GLShader *shader);

        bool link(GraphicAPI * ogl);

        const std::string getInfoLog() {
            std::string shadersInfoLog = "";
            std::vector<GLShader*>::iterator it;
            for(it = attachedShaders.begin(); it != attachedShaders.end(); it++) {
                shadersInfoLog += (*it)->getInfoLog();
            }
            return shadersInfoLog + "\n" + infoLog;
        }

        bool isLinked() {
            return linked;
        }
        
        int getOutputLocation(const std::string & name);

        const std::vector<Shader*> getShaderOfType(Shader::ShaderType type);

        static GLProgram * fromString(const std::string &vertexShader, const std::string &fragmentShader);

        static GLProgram * fromFile(FileReader * vertexShaderFile, FileReader *  fragmentShaderFile);

        GLuint getId() { return programID; }

    public: // Uniform binding
        void bindFloat(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size);

        void bindFloatVec2(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size);

        void bindFloatVec3(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size);

        void bindFloatVec4(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size);

        void bindBufferSampler(const UniformInfo & info, GraphicAPI * ogl, TextureBuffer * tex);

        void bindSampler1D(const UniformInfo & info, GraphicAPI * ogl, Texture1D * tex);

        void bindSampler2D(const UniformInfo & info, GraphicAPI * ogl, Texture2D * tex);

        void bindMat4(const UniformInfo & info, GraphicAPI * ogl, const float * v);

        void updateUniforms(GraphicAPI * ogl);
    private:
        void bindSampler(const UniformInfo & info, GraphicAPI * ogl, Texture * tex);

        void extractInfoLog();

        void extractUniformInformation(GraphicAPI * ogl);

        bool linked;

        GLuint programID;

        std::vector<GLShader*> attachedShaders;

        std::string infoLog;

        std::vector<std::string> output;

        std::vector<UniformInfo> uniforms;

        std::map<std::string, int> outputLocations;
    };
}

#endif
