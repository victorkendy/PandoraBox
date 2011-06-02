
#ifndef PBGE_GFX_FRAMEBUFFEROBJECT_H_
#define PBGE_GFX_FRAMEBUFFEROBJECT_H_

namespace pbge {
    class Texture2D;

    class FramebufferObject {
        virtual void addRenderable(Texture2D * texture) = 0;
        virtual void clearRenderables() = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
    };
}

#endif
