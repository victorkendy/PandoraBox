#ifndef OPENGLAPI_GFX_GLBLENDCONTROLLER_H
#define OPENGLAPI_GFX_GLBLENDCONTROLLER_H

#include <GL/glew.h>

#include "pbge/gfx/BlendController.h"

namespace pbge {
    class GLBlendController : public BlendController {
    public:
        GLBlendController();
        void enableBlending();
        void disableBlending();
        void useBlendFunc(BlendFunc srcFunc, BlendFunc dstFunc);
        void useBlendFuncSeparate(BlendFunc srcFunc, BlendFunc dstFunc, BlendFunc srcFuncAlpha, BlendFunc dstFuncAlpha);
        void useBlendEquation(BlendEquation equation);
    private:
        bool blendEnabled;
        GLenum translateBlendFunc(BlendController::BlendFunc func);
        BlendController::BlendFunc srcFunc;
        BlendController::BlendFunc dstFunc;
        BlendController::BlendFunc srcFuncAlpha;
        BlendController::BlendFunc dstFuncAlpha;
        BlendController::BlendEquation equation;
    };
}

#endif