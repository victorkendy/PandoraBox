#include <vector>

#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"


using namespace pbge;


void UpdaterVisitor::visit(Node * node, OpenGL * ogl) {
    node->updatePass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
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


void ColorPassVisitor::visit(Node *node, OpenGL *ogl) {
    node->renderPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postRenderPass(this, ogl);
}

void DepthPassVisitor::visit(Node *node, OpenGL *ogl) {
    node->depthPass(this, ogl);
    std::vector<Node*>::iterator child;
    for(child = node->getChilds().begin(); child != node->getChilds().end(); child++)
        this->visit(*child, ogl);
    node->postDepthPass(this, ogl);
}