#ifndef PBGE_GFX_OPENGL_GLPROGRAM_H
#define PBGE_GFX_OPENGL_GLPROGRAM_H

#include <string>
#include <map>
#include <vector>

#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/ShaderUniform.h"

#include "OpenGLAPI/gfx/GLShader.h"

namespace pbge {
    class GraphicAPI;
    class VertexBuffer;
    class FileReader;
    class AttrBinder;
    class BuiltInUniformBinder;

    class UniformBindAndInfo {
    public:
        UniformBindAndInfo(const UniformInfo & _info) {
            info = _info;
            value = NULL;
            stamp = 0;
        }
        UniformInfo getInfo() {
            return info;
        }
        bool shouldUpdate(UniformValue * newValue) {
            return newValue != NULL && ( (newValue != value) || (stamp != newValue->getStamp()) );
        }
        void update(UniformValue * newValue) {
            stamp = newValue->getStamp();
            value = newValue;
        }
        void reset() {
            stamp = 0;
            value = NULL;
        }
    private:
        UniformInfo info;
        UniformValue * value;
        unsigned long stamp;
    };

    class GLProgram : public GPUProgram{
    public:
        GLProgram() : programID(0), linked(false), currentVertexBuffer(NULL){
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

        int getOutputLocation(const std::string & name);

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

        void setAttributes(VertexBuffer * attr);
    private:
        void bindSampler(const UniformInfo & info, GraphicAPI * ogl, Texture * tex);

        void extractInfoLog();

        void extractUniformInformation(GraphicAPI * ogl);

        void extractAttribs();

        bool linked;

        GLuint programID;

        VertexBuffer * currentVertexBuffer;

        std::vector<GLShader*> attachedShaders;

        std::string infoLog;

        std::vector<std::string> output;

        std::vector<UniformBindAndInfo> uniforms;

        std::vector<AttrBinder *> attrBinders;

        std::vector<BuiltInUniformBinder*> builtInUniforms;

        std::map<std::string, int> outputLocations;
    };
}

#endif