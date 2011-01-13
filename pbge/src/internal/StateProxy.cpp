
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/internal/StateProxy.h"

using namespace pbge;

EnableMode::EnableMode(pbge::OpenGL::Mode _mode){
    this->mode = _mode;
}

void EnableMode::apply(pbge::OpenGL *ogl) {
    ogl->getState().enable(mode);
}   

void EnableMode::unApply(pbge::OpenGL *ogl) {
    ogl->getState().disable(mode);
}

DisableMode::DisableMode(pbge::OpenGL::Mode _mode) {
    this->mode = _mode;
}

void DisableMode::apply(pbge::OpenGL *ogl) {
    ogl->disable(mode);
}

void DisableMode::unApply(pbge::OpenGL *ogl) {
    ogl->enable(mode);
}