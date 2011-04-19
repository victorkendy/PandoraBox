
#ifndef gfx_rendertarget_h
#define gfx_rendertarget_h

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"

namespace pbge {

    class PBGE_EXPORT RenderTarget{
    public:
        RenderTarget():width(0),height(0) {}
        RenderTarget(int _width, int _height):width(_width), height(_height) {}
        virtual void setRenderTarget(GraphicAPI * ogl)=0;
        virtual void unsetRenderTarget(GraphicAPI * ogl){}
        GLint getWidth() const {
            return width;
        }
        GLint getHeight() const {
            return height; 
        }
    protected:
        GLint width;
        GLint height;
    };

    class PBGE_EXPORT Viewport: public RenderTarget{
    public:
        Viewport(GLint w, GLint h, GLint posX, GLint posY);

        void setRenderTarget(GraphicAPI * ogl);

        // Viewport does not need to do anything when unset is called
    private:
        GLint posx, posy;
    };
}
#endif
