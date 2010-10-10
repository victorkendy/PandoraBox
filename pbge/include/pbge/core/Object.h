

#ifndef _basicobject_h
#define _basicobject_h

#include "math3d/math3d.h"

namespace pbge {
    class Object{
    public:
        void * operator new(size_t s){
            return math3d::aligned_malloc(s, 16);
        }
    };
}
#endif