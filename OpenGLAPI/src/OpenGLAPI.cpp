#include "OpenGLAPI.h"
#include "OpenGLAPI/gfx/GLGraphic.h"

pbge::GraphicAPI * createInstance() {
    return new pbge::GLGraphic();
}