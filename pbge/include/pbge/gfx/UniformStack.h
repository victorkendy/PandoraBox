
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

        UniformSet * getGloabalUniforms();

        UniformValue * findUniform(const UniformInfo & info);

        void push(UniformSet * uniforms);

        void pop();

    private:

        std::vector<UniformSet *> stack;
    };
}

#endif
