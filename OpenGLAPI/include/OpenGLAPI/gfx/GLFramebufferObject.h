
#ifndef PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_
#define PBGE_GFX_OPENGL_GLFRAMEBUFFEROBJECT_H_

#include <cstddef>
#include <GL/glew.h>
#include <vector>
#include <boost/smart_ptr/scoped_array.hpp>
#include "pbge/gfx/FramebufferObject.h"

namespace pbge {
    class Texture2D;
    

    // TODO: think about renderable change...
    class GLFramebufferObject : public FramebufferObject {
    public:
        GLFramebufferObject(size_t w, size_t h):FramebufferObject(w,h),GLID(0),renderables(16,NULL),bindingPoints(new GLenum[16]){}
    protected:
        bool isInitialized();
        void initialize();
        void attachRenderable(Texture2D * texture);
        void dettachRenderable(Texture2D * texture);
		void attachDepthRenderable(Texture2D * tex);
        void bindFramebuffer();
        void unbindFramebuffer();
        void useRenderables(const std::vector<Texture2D *> & textures);
    private:
        GLuint GLID;
		int numberOfBindings;
		boost::scoped_array<GLenum> bindingPoints;
        std::vector<Texture2D*> renderables;
    };
}

#endif