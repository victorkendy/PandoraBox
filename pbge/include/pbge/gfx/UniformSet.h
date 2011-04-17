
#ifndef PBGE_GFX_UNIFORMSET_H
#define PBGE_GFX_UNIFORMSET_H

#include <map>
#include <string>

#include "pbge/core/core.h"
#include "pbge/gfx/ShaderUniform.h"

namespace pbge {
    
    class PBGE_EXPORT UniformSet {
    public:
        ~UniformSet();

        UniformValue * getValue(const std::string name, UniformType type);

        UniformValue * getValue(const UniformInfo & info);

        UniformValue * searchValue(const UniformInfo & info);

    private:
        UniformValue * createUniform(const UniformInfo & info);

        std::map<UniformInfo, UniformValue *> uniforms;
    };
}

#endif