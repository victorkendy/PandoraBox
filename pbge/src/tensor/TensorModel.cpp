#include "pbge/tensor/TensorModel.h"
#include "pbge/exceptions/exceptions.h"

using namespace pbge;

TensorModel::TensorModel(float * _tensor, int _order) {
    this->tensor = _tensor;
    this->order = _order;
    this->calculated = false;
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

void TensorModel::tensorIsSymmetric() {
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

}