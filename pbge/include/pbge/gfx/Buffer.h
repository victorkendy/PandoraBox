

#ifndef gfxbuffer_h_
#define gfxbuffer_h_

#include <cstdlib>
#include <iostream>

#include "pbge/core/Manager.h"

namespace pbge {

    

    // The Buffer Object interface
    class Buffer {
    public:

        typedef enum {
            READ_ONLY = 1,
            WRITE_ONLY,
            READ_WRITE
        } AccessPattern;

        typedef enum {
            VertexBuffer=4,
            IndexBuffer,
            PixelReadBackBuffer,
            PixelSendBuffer
        } Target;

        typedef enum {
            STREAM_DRAW = 8, 
            STREAM_READ, 
            STREAM_COPY,
            STATIC_DRAW, 
            STATIC_READ, 
            STATIC_COPY,
            DYNAMIC_DRAW, 
            DYNAMIC_READ,
            DYNAMIC_COPY
        } UsageHint;

        // allows factory method on OpenGL
        friend OpenGL;

        // returns the data buffer
        virtual void * map(AccessPattern access) = 0;

        virtual void unmap() = 0;

        virtual void unbind() = 0;

        virtual void bindOn(Target _target) = 0;

    protected:
        
        virtual void destroy() = 0;
    };

}

#endif