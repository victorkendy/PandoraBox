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
    
    /** A class to help users to define correct shaders.
        
        The ShaderHelper will use the same vertex shaders on both render pass and depth pass,
        which is the most common use case for rendering ModelInstance instances.
    */
    class PBGE_EXPORT ShaderHelper{
    public:
        /** The entry point for the class.
            @param gfx The graphic api implementation from which the default shader snippets will be
                   retrieved
            @return returns a instance of the ShaderHelper class allowing method chaining
        */
        static ShaderHelper * create(GraphicAPI * gfx);
    public:
        /** Add a vertex shader snippet definition.
            
            Exactly one of the added shader snippets must implement the function with the given prototype:
            void calculateVertex(out vec4 vertex, out vec3 normal, out vec4 color);

            @param shader A shader instance of type VERTEX_SHADER
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withVertexShader(Shader * shader);
        /**
            @param src The source code for the shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withVertexShader(const std::string & src);
        /**
            @param name The name of a registered shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withNamedVertexShader(const std::string & name);
        
        /** Add a fragment shader for the render pass.

            Exactly one of the added fragment shaders must implement the given prototype:
            void calculateFragmentColor(out vec4 color);

            @param shader A shader instance of type Shader::FRAGMENT_SHADER
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withRenderPassShader(Shader * shader);
        /**
            @param src The source code for the render pass fragment shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withRenderPassShader(const std::string & src);
        /**
            @param name The name of a registered fragment shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withNamedRenderPassShader(const std::string & name);
        
        /** Add a fragment shader to be used on the depth pass
            
            Exactly one of the added fragment shaders must implement the given prototype:
            void calculateDepth(in vec4 normalizedPosition, out float depth);

            @param shader A shader instance of type Shader::FRAGMENT_SHADER
        */
        ShaderHelper * withDepthPassShader(Shader * shader);
        /**
            @param src The source code for the depth pass fragment shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withDepthPassShader(const std::string & src);
        /**
            @param name The name of a registered fragment shader
            @return The same instance (this pointer) of the shader helper
        */
        ShaderHelper * withNamedDepthPassShader(const std::string & name);
        
        /** Sets the shaders to be used to render the instance
            
            @param instance A model instance object 
        */
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