#include <stdlib.h>
#include <math.h>

#include "pbge/tensor/TensorModel.h"
#include "pbge/exceptions/exceptions.h"
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
    if(this->order == 2) {
        this->calculateEigenValuesAndVectors2d();
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

void TensorModel::calculateEigenValuesAndVectors2d() {
    if(this->tensorIsDiagonal()) {
        this->eigenvalues = (float*)malloc(2*sizeof(float));
        this->eigenvalues[0] = this->tensor[0][0];
        this->eigenvalues[1] = this->tensor[1][1];

        this->eigenvectors = (float**)malloc(2*sizeof(float*));
        this->eigenvectors[0] = (float*)malloc(2*sizeof(float));
        this->eigenvectors[1] = (float*)malloc(2*sizeof(float));
        this->eigenvectors[0][0] = 1.0f;
        this->eigenvectors[0][1] = 0.0f;
        this->eigenvectors[1][0] = 0.0f;
        this->eigenvectors[1][1] = 1.0f;
    }
    else if(this->tensorIsSymmetric()) {
        this->calculateEigenValues2dSymmetric();
        this->calculateEigenVectors2dSymmetric();
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
    float eigenvalue1 = this->eigenvalues[0];
    float vector_entry = (dxx - eigenvalue1)/dxy;
    float magnitude = sqrt(1.0f + vector_entry*vector_entry);
    float normalized_entry1 = 1.0f/magnitude;
    float normalized_entry2 = vector_entry/magnitude;

    this->eigenvectors[0][0] = normalized_entry1;
    this->eigenvectors[0][1] = -normalized_entry2;
    this->eigenvectors[1][0] = normalized_entry2;
    this->eigenvectors[1][1] = normalized_entry1;
}

void TensorModel::render(ModelInstance * instance, OpenGL * ogl) {
    if(this->model == NULL) {
        if(this->order == 2) {
            float eigenvalue0 = getEigenvalue(0);
            float eigenvalue1 = getEigenvalue(1);
            this->model = new Ellipse(eigenvalue0, eigenvalue1, this->slices);
            float * eigenvector0 = getEigenvector(0);
            float * eigenvector1 = getEigenvector(1);
            math3d::matrix44 * rotation = new math3d::matrix44(eigenvector0[0], eigenvector1[0], 0, 0,
                                                               eigenvector0[1], eigenvector1[1], 0, 0,
                                                               0, 0, 1, 0,
                                                               0, 0, 0, 1);
            this->model->setTransformation(*rotation);
            this->model->render(instance, ogl);
        }
    }
    else {
        this->model->render(instance, ogl);
    }
}

void TensorModel::renderDepth(ModelInstance * instance, OpenGL * ogl) {}