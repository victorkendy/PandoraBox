#ifndef PBGE_GFX_SHADERHELPER_H_
#define PBGE_GFX_SHADERHELPER_H_

#include <string>
#include <vector>

#include "pbge/core/core.h"

namespace pbge {
    class GraphicAPI;
    class Shader;
    class GPUProgram;
    class ModelInstance;

    class PBGE_EXPORT ShaderHelper{
    public:
        static ShaderHelper * create(GraphicAPI * gfx);
    public:
        /* 
           one of the vertex shaders must implement the function 
           void calculateVertex(out vec4 vertex, out vec3 normal, out vec4 color);
        */
        ShaderHelper * withVertexShader(Shader * shader);
        ShaderHelper * withVertexShader(const std::string & src);
        ShaderHelper * withNamedVertexShader(const std::string & name);
        
        /*
          one of the fragment shaders for renderPass must implement
          void calculateFragmentColor(out vec4 color);
        */
        ShaderHelper * withRenderPassShader(Shader * shader);
        ShaderHelper * withRenderPassShader(const std::string & src);
        ShaderHelper * withNamedRenderPassShader(const std::string & name);
        
        /*
          one of the fragment shaders for depthPass must implement
          void calculateDepth(in vec4 normalizedPosition, out float depth);
        */
        ShaderHelper * withDepthPassShader(Shader * shader);
        ShaderHelper * withDepthPassShader(const std::string & src);
        ShaderHelper * withNamedDepthPassShader(const std::string & name);

        void setPrograms(ModelInstance * instance);
    private:
        ShaderHelper(GraphicAPI * gfx);
        GPUProgram * createProgram(const std::string & name, std::vector<Shader *> & shaders);
        Shader * searchShader(const std::string name);

        std::vector<Shader *> vertexShaders;
        std::vector<Shader *> depthPassShaders;
        std::vector<Shader *> renderPassShaders;
        GraphicAPI * gfx;
        GPUProgram * renderPassProgram;
        GPUProgram * depthPassProgram;
    };
}

#endif