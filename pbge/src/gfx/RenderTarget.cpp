#include <cstdio>
#include <GL/glew.h>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/RenderTarget.h"

namespace pbge {

    Viewport::Viewport(int w, int h, int posX, int posY):RenderTarget(w,h){
        this->posx = posX;
        this->posy = posY;
    }

    void Viewport::setRenderTarget(GraphicAPI * ogl){
        ogl->setViewport(this->posx, this->posy, getWidth(), getHeight());
    }
}