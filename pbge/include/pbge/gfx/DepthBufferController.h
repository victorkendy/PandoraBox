#ifndef PBGE_GFX_DEPTHBUFFERCONTROLLER_H
#define PBGE_GFX_DEPTHBUFFERCONTROLLER_H

namespace pbge {
    class DepthBufferController {
    public:
        typedef enum {
            DEPTH_LESS,
            DEPTH_LESS_EQUAL,
            DEPTH_GREATER,
            DEPTH_GREATER_EQUAL,
            DEPTH_NEVER,
            DEPTH_ALWAYS
        } DepthFunc;
        virtual void enableDepthTest() = 0;
        virtual void disableDepthTest() = 0;
        virtual void enableDepthWrite() = 0;
        virtual void disableDepthWrite() = 0;
        virtual void useDepthFunc(DepthFunc func) = 0;
    };
}

#endif