#include "pbge/exceptions/exceptions.h"
#include "OpenGLAPI/gfx/GLBlendController.h"

using namespace pbge;

GLBlendController::GLBlendController() :
    blendEnabled(false),
    srcFunc(BlendController::BLEND_FACTOR_ONE),
    dstFunc(BlendController::BLEND_FACTOR_ZERO),
    // What should be the default values here?
    srcFuncAlpha(BlendController::BLEND_FACTOR_ONE),
    dstFuncAlpha(BlendController::BLEND_FACTOR_ONE),
    equation(BlendController::BLEND_EQ_ADD) {}

void GLBlendController::enableBlending() {
    if(!blendEnabled) {
        glEnable(GL_BLEND);
        blendEnabled = true;
    }
}

void GLBlendController::disableBlending() {
    if(blendEnabled) {
        glDisable(GL_BLEND);
        blendEnabled = false;
    }
}

GLenum GLBlendController::translateBlendFunc(BlendController::BlendFunc func) {
    if(func == BlendController::BLEND_FACTOR_ONE)
        return GL_ONE;
    else if(func == BlendController::BLEND_FACTOR_ZERO)
        return GL_ZERO;
    else if(func == BlendController::BLEND_FACTOR_SRC_ALPHA)
        return GL_SRC_ALPHA;
    else if(func == BlendController::BLEND_FACTOR_DST_ALPHA)
        return GL_DST_ALPHA;
    else if(func == BlendController::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA)
        return GL_ONE_MINUS_SRC_ALPHA;
    else if(func == BlendController::BLEND_FACTOR_ONE_MINUS_DST_ALPHA)
        return GL_ONE_MINUS_DST_ALPHA;
    else
        throw IllegalArgumentException("Not a blend function");
}

void GLBlendController::useBlendFunc(BlendFunc newSrcFunc, BlendFunc newDstFunc) {
    if(newSrcFunc != srcFunc || newDstFunc != dstFunc) {
        glBlendFunc(translateBlendFunc(newSrcFunc), translateBlendFunc(newDstFunc));
    }
}

void GLBlendController::useBlendFuncSeparate(BlendFunc newSrcFunc, BlendFunc newDstFunc, BlendFunc newSrcFuncAlpha, BlendFunc newDstFuncAlpha) {
    if(newSrcFunc != srcFunc || newDstFunc != dstFunc || newSrcFuncAlpha != srcFuncAlpha || newDstFuncAlpha != dstFuncAlpha) {
        glBlendFuncSeparate(translateBlendFunc(newSrcFunc), 
                            translateBlendFunc(newDstFunc), 
                            translateBlendFunc(newSrcFuncAlpha), 
                            translateBlendFunc(newDstFuncAlpha));
    }
}

void GLBlendController::useBlendEquation(BlendEquation newEquation) {
    GLenum blendEq;
    if(newEquation != equation) {
        equation = newEquation;
        if(equation == BLEND_EQ_ADD)
            blendEq = GL_FUNC_ADD;
        else
            throw IllegalArgumentException("Not a blend equation");
        glBlendEquation(blendEq);
    }
}
