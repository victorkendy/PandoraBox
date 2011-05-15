
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

        UniformFloat * getFloat(const std::string & name);

        UniformFloatVec2 * getFloatVec2(const std::string & name);

        UniformFloatVec3 * getFloatVec3(const std::string & name);

        UniformFloatVec4 * getFloatVec4(const std::string & name);

        UniformMat4 * getFloatMat4(const std::string & name);
    private:
        UniformValue * createUniform(const UniformInfo & info);

        std::map<UniformInfo, UniformValue *> uniforms;
    };
}

#endif