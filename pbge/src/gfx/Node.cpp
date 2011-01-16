#include <vector>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

using namespace pbge;

void TransformationNode::updatePass(UpdaterVisitor *visitor, OpenGL *ogl) {
    visitor->pushTransformation(transformation);
}

void TransformationNode::postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    visitor->popTransformation();
}

