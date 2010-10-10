#include <cstdio>
#include <GL/glew.h>

#include "pbge/gfx/RenderTarget.h"

namespace pbge {

    Viewport::Viewport(GLint w, GLint h, GLint posX, GLint posY):RenderTarget(w,h){
        this->posx = posX;
        this->posy = posY;
    }

    void Viewport::setRenderTarget(OpenGL * ogl){
        ogl->viewport(this->posx, this->posy, getWidth(), getHeight());
    }
}