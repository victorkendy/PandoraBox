
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

bool TextureUnit::shouldChange(OpenGL * ogl) {
    return boundTexture != nextTexture;
}

void TextureUnit::makeChange(OpenGL * ogl) {
    
    ogl->activeTexture(GL_TEXTURE0 + index);
    if(nextTexture == NULL && boundTexture != NULL)
        ogl->bindTexture(boundTexture->getTarget(), 0);
    else
        nextTexture->bindTexture(ogl);
    boundTexture = nextTexture;
    
}

StateEnabler::StateEnabler(GLenum _mode) {
    this->current = this->next = false;
    this->mode = _mode;
}

bool StateEnabler::shouldChange(OpenGL * ogl) {
    return current != next;
}

void StateEnabler::makeChange(OpenGL * ogl) {
    if(next == true)
        ogl->enable(mode);
    else
        ogl->disable(mode);
    current = next;
}

void StateEnabler::enable() {
    this->next = true;
}

void StateEnabler::disable() {
    this->next = false;
}

bool BoundProgram::shouldChange(OpenGL * ogl) {
    return this->current != this->next;
}

void BoundProgram::makeChange(OpenGL * ogl) {
    if(this->next == NULL)
        ogl->useProgram(0);
    else
        next->bind(ogl);
    current = next;
}

void BoundProgram::updateUniforms(OpenGL * ogl) {
    if(this->current != NULL) {
        this->current->updateUniforms(ogl);
    }
}

