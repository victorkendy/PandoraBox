#include <vector>

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
    stackIndex = 0;
    transformationStack[0] = math3d::identity44;
    _visit(node, ogl);
}

void UpdaterVisitor::_visit(Node * node, GraphicAPI * ogl) {
    node->updatePass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
        this->_visit(*child, ogl);
    node->postUpdatePass(this, ogl);
}

void UpdaterVisitor::pushTransformation(const math3d::matrix44 & m) {
    transformationStack[++stackIndex] = m;
}

void UpdaterVisitor::popTransformation() {
    --stackIndex;
}

const math3d::matrix44 UpdaterVisitor::getCurrentTransformation() {
    return transformationStack[stackIndex];
}


void ColorPassVisitor::visit(Node *node, GraphicAPI *ogl) {
    node->renderPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
        this->visit(*child, ogl);
    node->postRenderPass(this, ogl);
}

void DepthPassVisitor::visit(Node *node, GraphicAPI *ogl) {
    node->depthPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
        this->visit(*child, ogl);
    node->postDepthPass(this, ogl);
}


void LightPassVisitor::visit(Node * node, pbge::GraphicAPI * ogl) {
    ogl->getState()->useProgram(currentLight->getLightPassProgram(ogl));
    currentLight->setNecessaryUniforms(ogl, currentCamera->getViewTransform());
    node->renderPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
        this->visit(*child, ogl);
    node->postRenderPass(this, ogl);
}