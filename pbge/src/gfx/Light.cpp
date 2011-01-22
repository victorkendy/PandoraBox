
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Light.h"

using namespace pbge;

void PointLight::updatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    visitor->addActiveLight(this);
}

void PointLight::renderPass(RenderVisitor * visitor, OpenGL * ogl) {
// TODO: Implement this!!
}

void PointLight::postRenderPass(RenderVisitor * visitor, OpenGL * ogl) {
// TODO: Implement this!!
}