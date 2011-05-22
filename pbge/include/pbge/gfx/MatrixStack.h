#ifndef PBGE_GFX_MATRIXSTACK_H_
#define PBGE_GFX_MATRIXSTACK_H_

#include "math3d/math3d.h"

namespace pbge {
    class MatrixStack {
    public:
        MatrixStack() {
            size = 16;
            index = -1;
            matrices = new math3d::matrix44[size];
        }
        ~MatrixStack() {
            delete [] matrices;
        }
        void push(const math3d::matrix44 & m);
        void pop();
        const math3d::matrix44 peek() const;
        void clear();
    private:
        void resize(void);

        unsigned index;

        unsigned size;
        
        math3d::matrix44 * matrices;
    };
}

#endif