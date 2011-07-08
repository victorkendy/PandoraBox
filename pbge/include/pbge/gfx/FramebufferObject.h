
#ifndef PBGE_GFX_FRAMEBUFFEROBJECT_H_
#define PBGE_GFX_FRAMEBUFFEROBJECT_H_

#include "pbge/core/core.h"

#include <cstddef>
#include <string>
#include <set>
#include <map>

namespace pbge {
    class Texture2D;
    
    // Experimental class with testable template methods
    class PBGE_EXPORT FramebufferObject {
    public:
        FramebufferObject(size_t w, size_t h):bound(false),width(w),height(h){}

        virtual ~FramebufferObject(){}

        void addRenderable(Texture2D * texture, const std::string & name) {
            renderables[name] = texture;
            unsync_added.insert(texture);
            if(isBound()) {
                validateAndAttachRenderable(texture);
                added.insert(texture);
            }
        }
        void removeRenderable(const std::string & name) {
            std::map<std::string,Texture2D *>::iterator i = renderables.find(name);
            if(i != renderables.end()) {
                unsync_added.erase(unsync_added.find(i->second));
                if(isBound()) {
                    added.erase(added.find(i->second));
                    dettachRenderable(i->second);
                }
                renderables.erase(name);
            }
        }
        void clearRenderables() {
            unsync_added.clear();
            renderables.clear();
        }
        void bind();
        void unbind();
        bool isBound() {
            return bound;
        }
    protected:
        virtual bool isInitialized() = 0;
        virtual void initialize() = 0;
        virtual void attachRenderable(Texture2D * texture) = 0;
        virtual void dettachRenderable(Texture2D * texture) = 0;
        virtual void bindFramebuffer() = 0;
        virtual void unbindFramebuffer() = 0;
    private:
		void validateAndAttachRenderable(Texture2D * tex);
        std::map<std::string,Texture2D *> renderables;
        std::set<Texture2D *> unsync_added;
        std::set<Texture2D*> added;
		size_t width, height;
        bool bound;
    };
}

#endif
