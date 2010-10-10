#include "pbge/gfx/Node.h"

using namespace pbge;

void NodeVisitor::visit(Node * node) {
    _visit(node);
}

void NodeVisitor::_visit(Node * node) {
    Node::node_list * childs = node->getChilds();
    Node::node_list::iterator child;
    doVisit(node);
    for(child = childs->begin(); child != childs->end(); child++)
        _visit(*child);
}

void UpdaterVisitor::doVisit(Node * node) {
    currentTransform = node->update(currentTransform);
}

math3d::matrix44 * Node::update(math3d::matrix44 * current) {
    return current;
}