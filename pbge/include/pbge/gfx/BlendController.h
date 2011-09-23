#ifndef PBGE_GFX_BLENDCONTROLLER_H
#define PBGE_GFX_BLENDCONTROLLER_H

namespace pbge {
    class BlendController {
    public:
        typedef enum {
            BLEND_FACTOR_ONE,
            BLEND_FACTOR_ZERO,
            BLEND_FACTOR_SRC_ALPHA,
            BLEND_FACTOR_DST_ALPHA,
            BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            BLEND_FACTOR_ONE_MINUS_DST_ALPHA
        } BlendFunc;
        typedef enum {
            BLEND_EQ_ADD
        } BlendEquation;
        virtual void enableBlending() = 0;
        virtual void disableBlending() = 0;
        virtual void useBlendFunc(BlendFunc srcFunc, BlendFunc dstFunc) = 0;
        virtual void useBlendEquation(BlendEquation equation) = 0;
    };
}

#endif