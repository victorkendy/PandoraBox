
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"


using namespace pbge;

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



bool BoundFBO::shouldChange(GraphicAPI * api) {
    return current != next;
}

void BoundFBO::makeChange(GraphicAPI * api) {
    if(current != NULL) {
        current->unbind();
    }
    if(next != NULL) {
        next->bind(api);
    }
    current = next;
}