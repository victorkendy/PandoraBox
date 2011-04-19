#include <stdlib.h>
#include <math.h>

#include "pbge/tensor/TensorModel.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/core/definitions.h"
#include "math3d/math3d.h"

using namespace pbge;

TensorModel::TensorModel(float ** _tensor, int _order, int _slices) {
    this->tensor = _tensor;
    this->order = _order;
    this->calculated = false;
    this->slices = _slices;
    this->model = NULL;
}

TensorModel::~TensorModel() {
    if(this->calculated)
        free(this->eigenvalues);
    for(int i = 0; i < order; i++)
        free(this->eigenvectors[i]);
    free(this->eigenvectors);
}

float TensorModel::getEigenvalue(int index) {
    if(index > this->order - 1 || index < 0) {
        throw InvalidIndexException("Invalid index for eigenvalue");
    }
    if(!this->calculated) {
        this->calculateEigenValuesAndVectors();
    }
    return this->eigenvalues[index];
}

float* TensorModel::getEigenvector(int index) {
    if(index > this->order - 1 || index < 0) {
        throw InvalidIndexException("Invalid index for eigenvector");
    }
    if(!this->calculated) {
        this->calculateEigenValuesAndVectors();
    }
    return this->eigenvectors[index];
}

void TensorModel::calculateEigenValuesAndVectors() {
    if(this->tensorIsDiagonal()) {
        this->calculateEigenValuesAndVectorsDiagonal();
    }
    else if(this->order == 2) {
        this->calculateEigenValuesAndVectors2d();
    }
    else if(this->order == 3) {
        this->calculateEigenValuesAndVectors3d();
    }
}

bool TensorModel::tensorIsDiagonal() {
    for(int i = 0; i < this->order; i++) {
        for(int j = 0; j < this->order; j++) {
            if(this->tensor[i][j] != 0.0f && i != j) {
                return false;
            }
        }
    }
    return true;
}

bool TensorModel::tensorIsSymmetric() {
    for(int i = 0; i < this->order; i++) {
        for(int j = i + 1; j < this->order; j++) {
            if(this->tensor[i][j] != this->tensor[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void TensorModel::calculateEigenValuesAndVectorsDiagonal() {
    int order = this->order;
    this->eigenvalues = (float*)malloc(order*sizeof(float));
    this->eigenvectors = (float**)malloc(order*sizeof(float*));

    for(int i = 0; i < order; i++) {
        this->eigenvalues[i] = this->tensor[i][i];
        this->eigenvectors[i] = (float*)malloc(order*sizeof(float));
        for(int j = 0; j < order; j++) {
            if(i == j) {
                this->eigenvectors[i][j] = 1.0f;
            }
            else {
                this->eigenvectors[i][j] = 0.0f;
            }
        }
    }
    this->calculated = true;
}

void TensorModel::calculateEigenValuesAndVectors2d() {
    if(this->tensorIsSymmetric()) {
        this->calculateEigenValues2dSymmetric();
        this->calculated = true;
        this->calculateEigenVectors2dSymmetric();
    }
}

void TensorModel::calculateEigenValuesAndVectors3d() {
    if(this->tensorIsSymmetric()) {
        this->calculateEigenValues3dSymmetric();
        this->calculated = true;
        this->calculateEigenVectors3dSymmetric();
    }
}

void TensorModel::calculateEigenValues2dSymmetric() {
    float dxx = this->tensor[0][0];
    float dyy = this->tensor[1][1];
    float dxy = this->tensor[0][1];

    this->eigenvalues = (float*)malloc(2*sizeof(float));
    this->eigenvalues[0] = (dxx + dyy + sqrt((dxx+dyy)*(dxx+dyy) - 4*(dxx*dyy - dxy*dxy)))/2;
    this->eigenvalues[1] = (dxx + dyy - sqrt((dxx+dyy)*(dxx+dyy) - 4*(dxx*dyy - dxy*dxy)))/2;
}

void TensorModel::calculateEigenVectors2dSymmetric() {
    this->eigenvectors = (float**)malloc(2*sizeof(float*));
    this->eigenvectors[0] = (float*)malloc(2*sizeof(float));
    this->eigenvectors[1] = (float*)malloc(2*sizeof(float));

    float dxx = this->tensor[0][0];
    float dxy = this->tensor[0][1];
    float eigenvalue1 = getEigenvalue(0);
    float vector_entry = (dxx - eigenvalue1)/dxy;
    float magnitude = sqrt(1.0f + vector_entry*vector_entry);
    float normalized_entry1 = 1.0f/magnitude;
    float normalized_entry2 = vector_entry/magnitude;

    this->eigenvectors[0][0] = normalized_entry1;
    this->eigenvectors[0][1] = -normalized_entry2;
    this->eigenvectors[1][0] = normalized_entry2;
    this->eigenvectors[1][1] = normalized_entry1;
}

void TensorModel::calculateEigenValues3dSymmetric() {
    float dxx = this->tensor[0][0];
    float dyy = this->tensor[1][1];
    float dzz = this->tensor[2][2];
    float dxy = this->tensor[0][1];
    float dxz = this->tensor[0][2];
    float dyz = this->tensor[1][2];
    float i1 = dxx + dyy + dzz;
    float i2 = dxx*dyy + dyy*dzz + dzz*dxx - (dxy*dxy + dxz*dxz + dyz*dyz);
    float i3 = dxx*dyy*dzz + 2*dxy*dxz*dyz - (dzz*dxy*dxy + dyy*dxz*dxz + dxx*dyz*dyz);
    float v_sqrt = sqrt((i1/3)*(i1/3) - i2/3);
    float s = (i1/3)*(i1/3)*(i1/3) - i1*i2/6 + i3/2;
    float phi = acos(s/(v_sqrt*v_sqrt*v_sqrt))/3;

    this->eigenvalues = (float*)malloc(3*sizeof(float));
    this->eigenvalues[0] = i1/3 + 2*v_sqrt*cos(phi);
    this->eigenvalues[1] = i1/3 - 2*v_sqrt*cos(PBGE_pi/3 + phi);
    this->eigenvalues[2] = i1/3 - 2*v_sqrt*cos(PBGE_pi/3 - phi);
}

void TensorModel::calculateEigenVectors3dSymmetric() {
    this->eigenvectors = (float**)malloc(3*sizeof(float*));
    this->eigenvectors[0] = (float*)malloc(3*sizeof(float));
    this->eigenvectors[1] = (float*)malloc(3*sizeof(float));
    this->eigenvectors[2] = (float*)malloc(3*sizeof(float));

    float dxx = this->tensor[0][0];
    float dyy = this->tensor[1][1];
    float dzz = this->tensor[2][2];
    float dxy = this->tensor[0][1];
    float dxz = this->tensor[0][2];
    float dyz = this->tensor[1][2];
    for(int i = 0; i < 3; i++) {
        float eigenvalue = getEigenvalue(i);
        float a = dxx - eigenvalue;
        float b = dyy - eigenvalue;
        float c = dzz - eigenvalue;
        float e_x = (dxy*dyz - b*dxz)*(dxz*dyz - c*dxy);
        float e_y = (dxz*dyz - c*dxy)*(dxy*dxz - a*dyz);
        float e_z = (dxy*dxz - a*dyz)*(dxy*dyz - b*dxz);
        float magnitude = sqrt(e_x*e_x + e_y*e_y + e_z*e_z);
        this->eigenvectors[i][0] = e_x/magnitude;
        this->eigenvectors[i][1] = e_y/magnitude;
        this->eigenvectors[i][2] = e_z/magnitude;
    }
}

void TensorModel::render(ModelInstance * instance, GraphicAPI * ogl) {
    if(this->model == NULL) {
        if(this->order == 2) {
            this->render2d(instance, ogl);
        }
        else if(this->order == 3) {
            this->render3d(instance, ogl);
        }
    }
    else {
        this->model->render(instance, ogl);
    }
}

void TensorModel::renderDepth(ModelInstance * instance, GraphicAPI * ogl) {}

void TensorModel::render2d(ModelInstance * instance, GraphicAPI * ogl) {
    float eigenvalues[2];
    eigenvalues[0] = getEigenvalue(0);
    eigenvalues[1] = getEigenvalue(1);
    this->model = new Ellipse(eigenvalues[0], eigenvalues[1], this->slices);
    float * eigenvectors[2];
    eigenvectors[0] = getEigenvector(0);
    eigenvectors[1] = getEigenvector(1);
    math3d::matrix44 * rotation = new math3d::matrix44(eigenvectors[0][0], eigenvectors[1][0], 0, 0,
                                                       eigenvectors[0][1], eigenvectors[1][1], 0, 0,
                                                       0, 0, 1, 0,
                                                       0, 0, 0, 1);
    dynamic_cast<Ellipse *>(this->model)->setTransformation(*rotation);
    this->model->render(instance, ogl);
}

void TensorModel::render3d(ModelInstance * instance, GraphicAPI * ogl) {
    float eigenvalues[3];
    eigenvalues[0] = getEigenvalue(0);
    eigenvalues[1] = getEigenvalue(1);
    eigenvalues[2] = getEigenvalue(2);
    this->model = new Ellipsoid(eigenvalues[0], eigenvalues[1], eigenvalues[2], this->slices);
    float * eigenvectors[3];
    eigenvectors[0] = getEigenvector(0);
    eigenvectors[1] = getEigenvector(1);
    eigenvectors[2] = getEigenvector(2);
    math3d::matrix44 * rotation = new math3d::matrix44(eigenvectors[0][0], eigenvectors[1][0], eigenvectors[2][0], 0,
                                                       eigenvectors[0][1], eigenvectors[1][1], eigenvectors[2][1], 0,
                                                       eigenvectors[0][2], eigenvectors[1][2], eigenvectors[2][2], 0,
                                                       0, 0, 0, 1);
    dynamic_cast<Ellipsoid *>(this->model)->setTransformation(*rotation);
    this->model->render(instance, ogl);
}