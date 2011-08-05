
#ifndef math3d_base
#define math3d_base 1

#include <iostream>

#include "math3d/core/core.h"

namespace math3d {
    class basicMath3dObject {
    public:
        void * operator new(size_t size){
            return internal_malloc(size, 16);
        }
        
        void operator  delete(void * p){
            internal_free(p);
        }

        /** Allocs a big chunk of memory with 16 bytes alignment

            return a 16 aligned memory chunk
        */
        void * operator new[] (size_t size) {
            return aligned_malloc(size, 16);
        }

        void operator delete[] (void * p) {
            aligned_free(p);
        }
    };
}

#endif 

