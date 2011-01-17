#include <vector>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

using namespace pbge;

TransformationNode * TransformationNode::translation(const float & x, const float & y, const float & z) {
    math3d::matrix44 m = math3d::identity44;
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
    return new TransformationNode(m); 
}

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