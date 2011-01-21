
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Shader.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"


using namespace pbge;

TextureUnit::TextureUnit(OpenGL * ogl, unsigned _index) {
    this->active = false;
    this->nextTexture = NULL;
    this->boundTexture = NULL;
    this->index = _index;
}

void TextureUnit::applyChanges(pbge::OpenGL *ogl) {
    if(boundTexture != nextTexture) {
        ogl->activeTexture(GL_TEXTURE0 + index);
        if(nextTexture == NULL)
            ogl->bindTexture(boundTexture->getTarget(), 0);
        else
            ogl->bindTexture(nextTexture->getTarget(), nextTexture->getGLID());
        boundTexture = nextTexture;
    }
}

StateEnabler::StateEnabler(GLenum _mode) {
    this->current = this->next = false;
    this->mode = _mode;
}

void StateEnabler::applyChanges(pbge::OpenGL * ogl) {
    if(current != next) {
        if(next == true)
            ogl->enable(mode);
        else
            ogl->disable(mode);
        current = next;
    }
}

void StateEnabler::enable() {
    this->next = true;
}

void StateEnabler::disable() {
    this->next = false;
}

void BoundProgram::applyChanges(OpenGL * ogl) {
    if(this->current != this->next) {
        if(this->next == NULL) 
            ogl->useProgram(0);
        else
            next->bind(ogl);
    }
    current = next;
}