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

}

#endif
