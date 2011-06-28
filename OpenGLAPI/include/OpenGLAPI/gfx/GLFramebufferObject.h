
#ifndef PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_
#define PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_

#include <GL/glew.h>
#include <vector>
#include "pbge/gfx/FramebufferObject.h"

namespace pbge {
    class Texture2D;

    class GLFramebufferObject : public FramebufferObject {
    public:
        GLFramebufferObject():FramebufferObject(),GLID(0),renderables(16,NULL){}
    protected:
        bool isInitialized();
        void initialize();
        void attachRenderable(Texture2D * texture);
        void dettachRenderable(Texture2D * texture);
        void bindFramebuffer();
        void unbindFramebuffer();
    private:
        GLuint GLID;
        std::vector<Texture2D*> renderables;
    };
}

#endif