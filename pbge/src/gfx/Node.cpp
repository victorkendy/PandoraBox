#include <vector>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

using namespace pbge;

void TransformationNode::updatePass(UpdaterVisitor *visitor, OpenGL *ogl) {
    visitor->pushTransformation(transformation);
}

void TransformationNode::postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    visitor->popTransformation();
}

void TransformationNode::renderPass(RenderVisitor * visitor, OpenGL * ogl) {
    ogl->loadModelMatrix(transformation);
}

CameraNode::CameraNode(Camera * _camera) {
    viewTransformation = math3d::identity44;
    this->camera = _camera;
    camera->setParent(this);
}

void CameraNode::updatePass(UpdaterVisitor * visitor, OpenGL * ogl) {
    viewTransformation = visitor->getCurrentTransformation();
    visitor->addActiveCamera(this->camera);
}