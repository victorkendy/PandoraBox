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

/*
void NodeVisitor::visit(Node * node) {
    _visit(node);
}

void NodeVisitor::_visit(Node * node) {
    Node::node_list & childs = node->getChilds();
    Node::node_list::iterator child;
    doVisit(node);
    for(child = childs.begin(); child != childs.end(); child++)
        _visit(*child);
}
*/