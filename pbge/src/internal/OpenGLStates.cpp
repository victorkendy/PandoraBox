
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"


using namespace pbge;

TextureUnit::TextureUnit(GraphicAPI * ogl, unsigned _index) {
    this->active = false;
    this->nextTexture = NULL;
    this->boundTexture = NULL;
    this->index = _index;
}

bool TextureUnit::shouldChange(GraphicAPI * ogl) {
    return boundTexture != nextTexture;
}

void TextureUnit::makeChange(GraphicAPI * ogl) {
    if(nextTexture == NULL && boundTexture != NULL) {
        // should unbind texture?
    } else {
        nextTexture->bindTextureOn(this);
    }
    boundTexture = nextTexture;
    
}

StateEnabler::StateEnabler(GLenum _mode) {
    this->current = this->next = false;
    this->mode = _mode;
}

bool StateEnabler::shouldChange(GraphicAPI * ogl) {
    return current != next;
}

void StateEnabler::makeChange(GraphicAPI * ogl) {
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

bool BoundProgram::shouldChange(GraphicAPI * ogl) {
    return this->current != this->next;
}

void BoundProgram::makeChange(GraphicAPI * ogl) {
    if(this->next == NULL)
        glUseProgram(0);
    else
        next->bind(ogl);
    current = next;
}

void BoundProgram::updateUniforms(GraphicAPI * ogl) {
    if(this->current != NULL) {
        this->current->updateUniforms(ogl);
    }
}

bool BoundFBO::shouldChange(GraphicAPI * api) {
    return current != next;
}

void BoundFBO::makeChange(GraphicAPI * api) {
    if(next != NULL) {
        next->bind(api);
    } else {
        current->unbind();
    }
    current = next;
}