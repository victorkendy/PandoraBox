
#ifndef PBGE_GFX_UNIFORMSTACK_H
#define PBGE_GFX_UNIFORMSTACK_H

#include <vector>
#include <string>

#include "pbge/core/core.h"

namespace pbge {
    class UniformSet;
    class UniformValue;
    class UniformFloat;
    class UniformFloatVec2;
    class UniformFloatVec3;
    class UniformFloatVec4;
    class UniformInfo;

    class PBGE_EXPORT UniformStack {
    public:
        UniformStack();

        ~UniformStack();

        UniformFloat * findFloat(const std::string & name);

        UniformFloatVec2 * findFloatVec2(const std::string & name);

        UniformFloatVec3 * findFloatVec3(const std::string & name);

        UniformFloatVec4 * findFloatVec4(const std::string & name);

        UniformSet * getGloabalUniforms();

        UniformValue * findUniform(const UniformInfo & info);

    private:

        std::vector<UniformSet *> stack;
    };
}

#endif
