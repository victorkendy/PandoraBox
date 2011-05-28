
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

        /** Get the uniform set at the bottom of the stack
            
            @return The bottom of the uniform stack
        */
        UniformSet * getGloabalUniforms();

        /** Finds the closest uniform on the stack that matches info.
            
            @param info A uniform variable specification
            @return The found uniform value (NULL if not found)
        */
        UniformValue * findUniform(const UniformInfo & info);

        /** Pushes a uniform set on top of the uniform stack.

            @param uniforms A uniform set to be pushed
        */
        void push(UniformSet * uniforms);

        /** Removes the top of the uniform stack
        */
        void pop();

    private:

        std::vector<UniformSet *> stack;
    };
}

#endif
