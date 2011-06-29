#ifndef PBGE_GFX_MATRIXSTACK_H_
#define PBGE_GFX_MATRIXSTACK_H_

#include <boost/smart_ptr/scoped_array.hpp>

#include "math3d/math3d.h"

#include "pbge/core/core.h"

namespace pbge {
    class PBGE_EXPORT MatrixStack {
    public:
        MatrixStack():size(16),index(-1),matrices(new math3d::matrix44[size]){
        }
        ~MatrixStack() {
        }
        void push(const math3d::matrix44 & m);
        void pop();
        const math3d::matrix44 peek() const;
        void clear();
    private:
        void resize(void);

        int index;

        unsigned size;
        
        boost::scoped_array<math3d::matrix44> matrices;
    };
}

#endif