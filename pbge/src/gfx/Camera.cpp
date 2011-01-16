#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>

#include "math3d/math3d.h"

#include "pbge/core/definitions.h"
#include "pbge/gfx/Camera.h"
//#include "pbge/gfx/Transformation.h"


using namespace pbge;

#include <iostream>

Camera::Camera(){
    this->name = "";
    this->lookAt(math3d::vector4(0,1,0,0), math3d::vector4(0,0,-1,0));
    parent = NULL;
    renderTarget = NULL;
}

Camera::Camera(const math3d::vector4 & up, const math3d::vector4 & front){
    lookAt(up, front);
    this->name = "";
    parent = NULL;
    renderTarget = NULL;    
}

void Camera::lookAt(const math3d::vector4 & up, const math3d::vector4 & front){
    frontVector = front;
    sideVector = math3d::cross(frontVector, up);
    upVector = math3d::cross(sideVector, frontVector);
    is_valid = 0;
}

void Camera::setCamera(OpenGL * ogl) {
    math3d::matrix44 viewMatrix;
    if(is_valid == 0) 
        setCameraTransformation();
    viewMatrix = cameraTransformation;
    if(this->parent != NULL) {
        viewMatrix *= this->parent->getViewTransformation().inverse();
    }
    ogl->loadViewMatrix(viewMatrix);
    
    this->frustum.loadProjection(ogl);
    if(this->renderTarget)
        this->renderTarget->setRenderTarget(ogl);
}

void Camera::unsetCamera(OpenGL * ogl){
    if(this->renderTarget)
        renderTarget->unsetRenderTarget(ogl);
}

void Camera::setCameraTransformation(){
    cameraTransformation.setRow(0,sideVector.normalize());
    cameraTransformation.setRow(1,upVector.normalize());
    cameraTransformation.setRow(2,-frontVector.normalize());
    cameraTransformation.setRow(3, math3d::vector4(0,0,0,1));
    is_valid = 1;
}


/****************************************************************************/

void Frustum::setProjectionMatrix(const math3d::matrix44 &newProjection) {
    this->projectionMatrix = newProjection;
}

void Frustum::setFrustum(const float &left, const float &right,
                         const float &bottom, const float &top,
                         const float &near, const float &far) {
    
    float A = (right + left) / (right - left),
          B = (top + bottom) / (top - bottom),
          C = -(far + near) / (far - near),
          D = -(2*far*near) / (far - near);
    projectionMatrix.loadIdentity ();
    projectionMatrix[0][0] = 2 * near / (right - left);
    projectionMatrix[1][1] = 2 * near / (top - bottom);
    /* Put A, B, C, and D in the projection matrix */
    projectionMatrix[0][2] = A;
    projectionMatrix[1][2] = B;
    projectionMatrix[2][2] = C;
    projectionMatrix[3][2] = -1;
    projectionMatrix[2][3] = D;
    projectionMatrix[3][3] = 0;

    updateFrustumPlanes();
}

void Frustum::setOrtho(const float &left, const float &right, 
                       const float &bottom, const float &top, 
                       const float &near, const float &far) {

    float right_left = right - left,
          top_bottom = top - bottom,
          far_near = far - near;
    projectionMatrix.loadIdentity();
    projectionMatrix[0][0] = 2 / right_left;
    projectionMatrix[1][1] = 2 / top_bottom;
    projectionMatrix[2][2] = -2.0f / far_near;
    
    projectionMatrix[0][3] = -(right + left) / right_left;
    projectionMatrix[1][3] = -(top + bottom) / top_bottom;
    projectionMatrix[2][3] = -(far + near) / far_near;
    
    updateFrustumPlanes();
}


void Frustum::setPerspective(const float &fovy, const float &aspect, const float &near, const float &far) {
    float cotan = 1.0f / static_cast<float>(tan (degToRad(fovy/2.0f)));
    float near_far = near - far;
    projectionMatrix = math3d::zeroMatrix44;
    projectionMatrix[0][0] = cotan / aspect;
    projectionMatrix[1][1] = cotan;
    projectionMatrix[2][2] = (near + far) / near_far;
    projectionMatrix[3][2] = -1.0f;
    projectionMatrix[2][3] = 2 * near * far / near_far;
    updateFrustumPlanes();
}

void Frustum::loadProjection(OpenGL * ogl) const {
    ogl->loadProjectionMatrix(projectionMatrix);
}

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


