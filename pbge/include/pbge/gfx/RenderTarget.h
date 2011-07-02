
#ifndef gfx_rendertarget_h
#define gfx_rendertarget_h

#include "pbge/core/core.h"

namespace pbge {
    
    class GraphicAPI;

    class PBGE_EXPORT RenderTarget{
    public:
        RenderTarget():width(0),height(0) {}
        RenderTarget(int _width, int _height):width(_width), height(_height) {}
        virtual void setRenderTarget(GraphicAPI * ogl)=0;
        virtual void unsetRenderTarget(GraphicAPI * ogl){}
        int getWidth() const {
            return width;
        }
        int getHeight() const {
            return height; 
        }
    protected:
        int width;
        int height;
    };

    class PBGE_EXPORT Viewport: public RenderTarget{
    public:
        Viewport(int w, int h, int posX, int posY);

        void setRenderTarget(GraphicAPI * ogl);

        // Viewport does not need to do anything when unset is called
    private:
        int posx, posy;
    };
}
#endif
