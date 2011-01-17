#include <vector>
#include <cmath>

#include "math3d/utils/matrix_utils.h" 

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"

using namespace pbge;

TransformationNode * TransformationNode::translation(const float & x, const float & y, const float & z) {
    return new TransformationNode(math3d::translationMatrix(x,y,z)); 
}

TransformationNode * TransformationNode::scaling(const float & sx, const float & sy, const float & sz) {
    return new TransformationNode(math3d::scaleMatrix(sx,sy,sz));
}

TransformationNode * TransformationNode::rotation(const float & angle, const float & x, const float & y, const float & z) {
    return new TransformationNode(math3d::rotationMatrix(angle, x, y, z));
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