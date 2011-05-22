#include <vector>
#include <cmath>

#include "math3d/utils/matrix_utils.h" 

#include "pbge/gfx/GraphicAPI.h"
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

void TransformationNode::updatePass(UpdaterVisitor *visitor, GraphicAPI *ogl) {
    *current = visitor->getCurrentTransformation() * (*transformation);
    visitor->pushTransformation(*current);
}

void TransformationNode::postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {
    visitor->popTransformation();
}

void TransformationNode::renderPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    visitor->pushTransformation(*current);
    ogl->loadModelMatrix(*current);
}

void TransformationNode::postRenderPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    visitor->popTransformation();
    gfx->loadModelMatrix(visitor->getCurrentTransformation());
}

void TransformationNode::depthPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    visitor->pushTransformation(*current);
    ogl->loadModelMatrix(*current);
}

void TransformationNode::postDepthPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    visitor->popTransformation();
    gfx->loadModelMatrix(visitor->getCurrentTransformation());
}

TransformationNode * TransformationNode::scale(const float & sx, const float & sy, const float & sz) {
    *transformation *= math3d::scaleMatrix(sx, sy, sz);
    return this;
}

TransformationNode * TransformationNode::rotate(const float & radAngle, const float & x, const float & y, const float & z) {
    *transformation *= math3d::rotationMatrix(radAngle, x, y, z);
    return this;
}

TransformationNode * TransformationNode::translate(const float & x, const float & y, const float & z) {
    *transformation *= math3d::translationMatrix(x,y,z);
    return this;
}


CameraNode::CameraNode() {
    viewTransformation = new math3d::matrix44(math3d::identity44);
    this->camera = new Camera();
    camera->setParent(this);
}

CameraNode::CameraNode(Camera * _camera) {
    *viewTransformation = math3d::identity44;
    this->camera = _camera;
    camera->setParent(this);
}

void CameraNode::updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {
    *viewTransformation = visitor->getCurrentTransformation();
    visitor->addActiveCamera(this->camera);
}

void CameraNode::lookAt(const math3d::vector4 & up, const math3d::vector4 & front) {
    this->camera->lookAt(up, front);
}

void CameraNode::setPerspective(const float & fovy, const float & aspect,
                                const float & near, const float & far) {
    this->camera->frustum.setPerspective(fovy, aspect, near, far);
}