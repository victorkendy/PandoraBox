
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/internal/StateProxy.h"

using namespace pbge;

EnableMode::EnableMode(pbge::GraphicAPI::Mode _mode){
    this->mode = _mode;
}

void EnableMode::apply(pbge::GraphicAPI *ogl) {
    ogl->getState()->enable(mode);
}   

void EnableMode::unApply(pbge::GraphicAPI *ogl) {
    ogl->getState()->disable(mode);
}

DisableMode::DisableMode(pbge::GraphicAPI::Mode _mode) {
    this->mode = _mode;
}

void DisableMode::apply(pbge::GraphicAPI *ogl) {
    ogl->disable(mode);
}

void DisableMode::unApply(pbge::GraphicAPI *ogl) {
    ogl->enable(mode);
}