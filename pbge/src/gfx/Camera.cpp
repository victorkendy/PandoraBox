#include <GL/glew.h>
#include <cmath>


#include "math3d/math3d.h"

#include "pbge/core/definitions.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Camera.h"

using namespace pbge;

Camera::Camera():parent(NULL),viewport(NULL),
                 upVector(new math3d::vector4),sideVector(new math3d::vector4),frontVector(new math3d::vector4),
                 cameraTransformation(new math3d::matrix44),viewTransform(new math3d::matrix44) {
    this->lookAt(math3d::vector4(0,1,0,0), math3d::vector4(0,0,-1,0));
}

Camera::Camera(const math3d::vector4 & up, const math3d::vector4 & front):parent(NULL),viewport(NULL),
                 upVector(new math3d::vector4(up)), sideVector(new math3d::vector4),frontVector(new math3d::vector4(front)),
                 cameraTransformation(new math3d::matrix44),viewTransform(new math3d::matrix44) {
    lookAt(up, front);
}

Camera::~Camera() {
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
    math3d::matrix44 projection = frustum.getProjectionMatrix();
    if(this->parent != NULL) {
        *viewTransform *= this->parent->getViewTransformation().inverse();
    }
    ogl->loadViewMatrix(*viewTransform);
    ogl->loadProjectionMatrix(projection);
    // I know it seems backwards but that is the way the projection * view is usually called
    math3d::matrix44 viewProjection = projection * (*viewTransform);
    boundingFrustum.setViewProjection(viewProjection);
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
}

void Frustum::setProjectionMatrix(const math3d::matrix44 &newProjection) {
    *(this->projectionMatrix) = newProjection;
}

void Frustum::setFrustum(const float &left, const float &right,
                         const float &bottom, const float &top,
                         const float &near, const float &far) {
    *projectionMatrix = math3d::frustumMatrix(left, right, bottom, top, near, far);
}

void Frustum::setOrtho(const float &left, const float &right, 
                       const float &bottom, const float &top, 
                       const float &near, const float &far) {
    *projectionMatrix = math3d::orthographicMatrix(left, right, bottom, top, near, far);
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
}
