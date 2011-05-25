#include <GL/glew.h>
#include <cmath>

#include "math3d/math3d.h"

#include "pbge/core/definitions.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Camera.h"

using namespace pbge;

Camera::Camera(){
    this->upVector = new math3d::vector4;
    this->sideVector = new math3d::vector4;
    this->frontVector = new math3d::vector4;
    this->cameraTransformation = new math3d::matrix44;
    this->viewTransform = new math3d::matrix44;
    this->lookAt(math3d::vector4(0,1,0,0), math3d::vector4(0,0,-1,0));
    parent = NULL;
    viewport = NULL;
}

Camera::Camera(const math3d::vector4 & up, const math3d::vector4 & front){
    this->upVector = new math3d::vector4(up);
    this->sideVector = new math3d::vector4;
    this->frontVector = new math3d::vector4(front);
    this->cameraTransformation = new math3d::matrix44;
    this->viewTransform = new math3d::matrix44;
    lookAt(up, front);
    parent = NULL;
    viewport = NULL;    
}

Camera::~Camera() {
    delete upVector;
    delete sideVector;
    delete frontVector;
    delete cameraTransformation;
    delete viewTransform;
}

void Camera::lookAt(const math3d::vector4 & up, const math3d::vector4 & front){
    *frontVector = front;
    *sideVector = math3d::cross(*frontVector, up);
    *upVector = math3d::cross(*sideVector, *frontVector);
    is_valid = 0;
}

void Camera::setCamera(GraphicAPI * ogl) {
    if(is_valid == 0) 
        setCameraTransformation();
    *viewTransform = *cameraTransformation;
    if(this->parent != NULL) {
        *viewTransform *= this->parent->getViewTransformation().inverse();
    }
    ogl->loadViewMatrix(*viewTransform);
    
    this->frustum.loadProjection(ogl);
    if(this->viewport)
        this->viewport->setRenderTarget(ogl);
}

void Camera::unsetCamera(GraphicAPI * ogl){
    if(this->viewport)
        viewport->unsetRenderTarget(ogl);
}

void Camera::setCameraTransformation(){
    cameraTransformation->setRow(0,sideVector->normalize());
    cameraTransformation->setRow(1,upVector->normalize());
    cameraTransformation->setRow(2,-frontVector->normalize());
    cameraTransformation->setRow(3, math3d::vector4(0,0,0,1));
    is_valid = 1;
}


/****************************************************************************/

Frustum::~Frustum() {
    delete [] frustumPlanes;
    delete [] frustumPoints;
    delete projectionMatrix;
}

void Frustum::setProjectionMatrix(const math3d::matrix44 &newProjection) {
    *(this->projectionMatrix) = newProjection;
}

void Frustum::setFrustum(const float &left, const float &right,
                         const float &bottom, const float &top,
                         const float &near, const float &far) {
    *projectionMatrix = math3d::frustumMatrix(left, right, bottom, top, near, far);
    updateFrustumPlanes();
}

void Frustum::setOrtho(const float &left, const float &right, 
                       const float &bottom, const float &top, 
                       const float &near, const float &far) {
    *projectionMatrix = math3d::orthographicMatrix(left, right, bottom, top, near, far);
    updateFrustumPlanes();
}


void Frustum::setPerspective(const float &fovy, const float &aspect, const float &near, const float &far) {
    float cotan = 1.0f / static_cast<float>(tan (degToRad(fovy/2.0f)));
    float near_far = near - far;
    (*projectionMatrix) = math3d::zeroMatrix44;
    (*projectionMatrix)[0][0] = cotan / aspect;
    (*projectionMatrix)[1][1] = cotan;
    (*projectionMatrix)[2][2] = (near + far) / near_far;
    (*projectionMatrix)[3][2] = -1.0f;
    (*projectionMatrix)[2][3] = 2 * near * far / near_far;
    updateFrustumPlanes();
}

void Frustum::loadProjection(GraphicAPI * ogl) const {
    ogl->loadProjectionMatrix(*projectionMatrix);
}

// TODO: Implement the methods below
void Frustum::updatePerspectivePoints() {

}

void Frustum::updateOrthoPoints() {

}

void Frustum::updateFrustumPoints() {

}

// Extract the frustum planes from the projectionMatrix
void Frustum::updateFrustumPlanes() {
    /*
    // Left plane
    frustumPlanes[0] = normalizePlane(projectionMatrix[3] + projectionMatrix[0]);
    // Right plane
    frustumPlanes[1] = normalizePlane(projectionMatrix[3] - projectionMatrix[0]);
    // Bottom plane
    frustumPlanes[2] = normalizePlane(projectionMatrix[3] + projectionMatrix[1]);
    // Top plane
    frustumPlanes[3] = normalizePlane(projectionMatrix[3] - projectionMatrix[1]);
    // Near plane
    frustumPlanes[4] = normalizePlane(projectionMatrix[3] + projectionMatrix[2]);
    // Far plane
    frustumPlanes[5] = normalizePlane(projectionMatrix[3] - projectionMatrix[2]);
    */
}


