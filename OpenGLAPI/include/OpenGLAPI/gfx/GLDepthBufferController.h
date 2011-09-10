
#ifndef OPENGLAPI_GFX_GLDEPTHBUFFERCONTROLLER_H
#define OPENGLAPI_GFX_GLDEPTHBUFFERCONTROLLER_H

#include "pbge/gfx/DepthBufferController.h"

namespace pbge {
    class GLDepthBufferController : public DepthBufferController {
    public:
        GLDepthBufferController();
        void enableDepthTest();
        void disableDepthTest();
        void enableDepthWrite();
        void disableDepthWrite();
        void useDepthFunc(DepthFunc func);
    private:
        bool writeEnabled;

        bool testEnabled;

        DepthBufferController::DepthFunc depthFunc;
    };
}

#endif