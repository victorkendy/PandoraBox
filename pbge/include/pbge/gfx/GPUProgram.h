#ifndef PBGE_GFX_GPUPROGRAM_H
#define PBGE_GFX_GPUPROGRAM_H

#include <string>
#include <vector>

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Shader.h"

namespace pbge {
    class Texture1D;
    class Texture2D;
    class TextureBuffer;
    class UniformInfo;

    class PBGE_EXPORT GPUProgram {
    public:
        virtual void bind(GraphicAPI * ogl) = 0;

        virtual void unbind(GraphicAPI * ogl) = 0;

        virtual bool link(GraphicAPI * ogl) = 0;

        virtual const std::string getInfoLog() = 0;

        virtual unsigned getId() = 0;

        virtual void updateUniforms(GraphicAPI * ogl) = 0;

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
}


#endif