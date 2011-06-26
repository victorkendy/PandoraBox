
#ifndef PBGE_GFX_FRAMEBUFFEROBJECT_H_
#define PBGE_GFX_FRAMEBUFFEROBJECT_H_

#include "pbge/core/core.h"

#include <string>
#include <set>
#include <map>

namespace pbge {
    class Texture2D;
    
    // Experimental class with testable template methods
    class PBGE_EXPORT FramebufferObject {
    public:
        void addRenderable(Texture2D * texture, const std::string & name) {
            renderables[name] = texture;
        }
        void removeRenderable(const std::string & name) {
            renderables.erase(name);
        }
        void clearRenderables() {
            renderables.clear();
        }
        void bind();
        void unbind();
    protected:
        virtual bool isInitialized() = 0;
        virtual void initialize() = 0;
        virtual void attachRenderable(Texture2D * texture) = 0;
        virtual void dettachRenderable(Texture2D * texture) = 0;
        virtual void bindFramebuffer() = 0;
        virtual void unbindFramebuffer() = 0;
    private:
        std::map<std::string,Texture2D *> renderables;
        std::set<Texture2D*> toAdd;
    };
}

#endif
