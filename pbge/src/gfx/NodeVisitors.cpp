#include <vector>

#include "pbge/gfx/MatrixStack.h"
#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Light.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/NodeVisitors.h"


using namespace pbge;

void UpdaterVisitor::visit(Node * node, GraphicAPI * ogl) {
    // reset the visitor state
    activeCameras.clear();
    activeLights.clear();
    trait.clearMatrixStack();
    trait.visit(node, ogl, this);
}

void LightPassVisitor::preVisit(GraphicAPI* api) {
    api->getState()->useProgram(currentLight->getLightPassProgram(api));
    currentLight->setNecessaryUniforms(api, currentCamera->getViewTransform());
}
