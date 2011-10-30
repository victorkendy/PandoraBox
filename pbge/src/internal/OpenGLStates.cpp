
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"


using namespace pbge;


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