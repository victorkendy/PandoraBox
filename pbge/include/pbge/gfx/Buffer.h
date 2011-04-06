

#ifndef gfxbuffer_h_
#define gfxbuffer_h_

#include <cstdlib>
#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/gfx/OpenGL.h"

namespace pbge {

    // The Buffer Object interface
    class Buffer {
    public:
        // allows factory method on OpenGL
        friend OpenGL;

        // returns the data buffer
        virtual void * map(GLenum access) = 0;

        virtual void unbind() = 0;

        virtual void bindOn(GLenum _target) = 0;

    protected:
        
        virtual void destroy() = 0;
    };

}

#endif