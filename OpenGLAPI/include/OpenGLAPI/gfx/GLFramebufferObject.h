
#ifndef PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_
#define PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_

#include "pbge/gfx/FramebufferObject.h"

namespace pbge {
    class Texture2D;

    class GLFramebufferObject : public FramebufferObject {
        void clearRenderables();

        void bind();

        void unbind();
    };
}

#endif