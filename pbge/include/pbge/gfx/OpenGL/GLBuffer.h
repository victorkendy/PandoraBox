

#ifndef PBGE_GRAPHIC_API_GL_BUFFER
#define PBGE_GRAPHIC_API_GL_BUFFER

#include <GL/glew.h>
#include "pbge/gfx/Buffer.h"

namespace pbge {
    class GraphicAPI;
    class GLObjectsFactory;


    // TODO: make changes to the buffer interface
    class GLBuffer : Buffer {
    public:
        friend GLObjectsFactory;

        // returns the data buffer
        void * map(Buffer::AccessPattern access);

        void unmap();

        void unbind();

        void bindOn(Buffer::Target _target);

        GLuint getID() {
            return glID;
        }
    private:
        // retirei o target
        GLBuffer(size_t _size, Buffer::UsageHint _usage, GraphicAPI * _gl);
        
        void destroy();

        void initialize();
        
        GraphicAPI * gl;

        GLenum usage;

        GLenum target;

        void * data;

        GLuint glID;

        size_t size;
    };
}



#endif