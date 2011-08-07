

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
            PixelSendBuffer,
            BufferTextureStorage
        } Target;

        typedef enum {
            STREAM_DRAW = 9, 
            STREAM_READ, 
            STREAM_COPY,
            STATIC_DRAW, 
            STATIC_READ, 
            STATIC_COPY,
            DYNAMIC_DRAW, 
            DYNAMIC_READ,
            DYNAMIC_COPY
        } UsageHint;

        /** maps the buffer's memory on the client's memory space.
            
            @param access The intended usage pattern for the mapped area
        */
        virtual void * map(AccessPattern access) = 0;

        /** returns a previously mapped client memory to the graphic api.
        */
        virtual void unmap() = 0;

        /** Removes the binding between a buffer and a target
        */
        virtual void unbind() = 0;

        /** Bind the buffer to a specific target
            
            @param target for the buffer binding
        */
        virtual void bindOn(Target target) = 0;

    protected:
      
        /** Release the api specific buffer object
        */
        virtual void destroy() = 0;
    };

}

#endif