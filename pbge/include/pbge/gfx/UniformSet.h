
#ifndef PBGE_GFX_UNIFORMSET_H
#define PBGE_GFX_UNIFORMSET_H

#include <map>
#include <string>

#include "pbge/core/core.h"
#include "pbge/gfx/ShaderUniform.h"

namespace pbge {
    /** A uniform cache

        @author Victor Kendy Harada
    */
    class PBGE_EXPORT UniformSet {
    public:
        ~UniformSet();

        /** Gets a shader uniform value.

            Search the internal uniform cache, if the specified uniform is not found it returns a new instance
            of the uniform with the name and type specified, if the uniform is found it returns the found instance
            
            @param name The name of the uniform
            @param type The type of the uniform
            @return A uniform with the specified name and type
        */
        UniformValue * getValue(const std::string name, UniformType type);
    
        /** Same as getValue(info.getName(), info.getType()).
            
            @param info The uniform name and type
            @return A uniform with the specified name and type
        */
        UniformValue * getValue(const UniformInfo & info);
        
        /** Gets a shader uniform value.

            Search the internal uniform cache, if the specified uniform is not found it returns NULL,
            if the uniform is found it returns the found instance

            @param name The name of the uniform
            @param type The type of the uniform
            @return A uniform with the specified name and type
        */
        UniformValue * searchValue(const UniformInfo & info);

        /**
            @param name The name of a uniform of type FLOAT
            @return A instance of UniformFloat
        */
        UniformFloat * getFloat(const std::string & name);
        
        /**
            @param name The name of a uniform of type FLOAT_VEC2
            @return A instance of UniformFloatVec2
        */
        UniformFloatVec2 * getFloatVec2(const std::string & name);
        
        /**
            @param name The name of a uniform of type FLOAT_VEC3
            @return A instance of UniformFloatVec3
        */
        UniformFloatVec3 * getFloatVec3(const std::string & name);

        /**
            @param name The name of a uniform of type FLOAT_VEC4
            @return A instance of UniformFloatVec4
        */
        UniformFloatVec4 * getFloatVec4(const std::string & name);

        /**
            @param name The name of a uniform of type FLOAT_MAT4
            @return A instance of UniformMat4
        */
        UniformMat4 * getFloatMat4(const std::string & name);

        UniformBufferSampler * getBufferSampler(const std::string & name);
        /**
            @param name The name of a uniform of type SAMPLER1D
            @return A instance of UniformSampler1D
        */
        UniformSampler1D * getSampler1D(const std::string & name);

        /**
            @param name The name of a uniform of type SAMPLER2D
            @return A instance of UniformSampler2D
        */
        UniformSampler2D * getSampler2D(const std::string & name);
    private:
        /** Instantiate and store a uniform specified by info

            @param info The information of the uniform to be instanced
            @return A uniform value instance
        */
        UniformValue * createUniform(const UniformInfo & info);

        std::map<UniformInfo, UniformValue *> uniforms;
    };
}

#endif