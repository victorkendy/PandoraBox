#include <cmath>

#include "TensorFactory.h"
#include "Ellipsoids.h"

#define TENSOR_FACTORY_PI 3.14159f

Tensor3DProcessor::Tensor3DProcessor(float * _tensor) {
    this->tensor = _tensor;
    calculateEigenValues();
    calculateEigenVectors();
}

void Tensor3DProcessor::calculateEigenValues() {
    if(isTensorDiagonal()) {
        calculateEigenValuesForDiagonalTensor();
    }
    else {
        calculateEigenValuesForNonDiagonalTensor();
    }
}

void Tensor3DProcessor::calculateEigenValuesForDiagonalTensor() {
	int positions[3] = {0, 3, 5};
	for(int i = 0; i < 3; i++) {
        float value = this->tensor[positions[i]];
        //Should this happen?
        if(value == 0) value = 0.05f;
        this->eigenvalues[i] = value;
    }
}

void Tensor3DProcessor::calculateEigenValuesForNonDiagonalTensor() {
    float dxx = this->tensor[0];
    float dyy = this->tensor[3];
    float dzz = this->tensor[5];
    float dxy = this->tensor[1];
    float dxz = this->tensor[2];
    float dyz = this->tensor[4];
	float i1 = dxx + dyy + dzz;
    float i2 = dxx*dyy + dyy*dzz + dzz*dxx - (dxy*dxy + dxz*dxz + dyz*dyz);
    float i3 = dxx*dyy*dzz + 2*dxy*dxz*dyz - (dzz*dxy*dxy + dyy*dxz*dxz + dxx*dyz*dyz);
    float v_sqrt = sqrt((i1/3)*(i1/3) - i2/3);
    float s = (i1/3)*(i1/3)*(i1/3) - i1*i2/6 + i3/2;
    float phi_aux = s/(v_sqrt*v_sqrt*v_sqrt);
    //Adjustment due to precision problems for acos function
    if(phi_aux > 1.0) phi_aux = 1.0f;
    else if(phi_aux < -1.0) phi_aux = -1.0f;
    float phi = acos(phi_aux)/3;

	this->eigenvalues[0] = i1/3 + 2*v_sqrt*cos(phi);
    this->eigenvalues[1] = i1/3 - 2*v_sqrt*cos(TENSOR_FACTORY_PI/3 + phi);
    this->eigenvalues[2] = i1/3 - 2*v_sqrt*cos(TENSOR_FACTORY_PI/3 - phi);
}

bool Tensor3DProcessor::isTensorDiagonal() {
    return tensor[1] == 0.0f && tensor[2] == 0.0f && tensor[4] == 0.0f;
}

void Tensor3DProcessor::calculateEigenVectors() {
    if(isTensorDiagonal()) {
        calculateEigenVectorsForDiagonalTensor();
    }
    else {
        calculateEigenVectorsForNonDiagonalTensor();
    }
}

void Tensor3DProcessor::calculateEigenVectorsForDiagonalTensor() {
    this->eigenvector1[0] = 1.0f;
    this->eigenvector1[1] = 0.0f;
    this->eigenvector1[2] = 0.0f;
    this->eigenvector2[0] = 0.0f;
    this->eigenvector2[1] = 1.0f;
    this->eigenvector2[2] = 0.0f;
    this->eigenvector3[0] = 0.0f;
    this->eigenvector3[1] = 0.0f;
    this->eigenvector3[2] = 1.0f;
}

void Tensor3DProcessor::calculateEigenVectorsForNonDiagonalTensor() {
    float dxx = this->tensor[0];
    float dyy = this->tensor[3];
    float dzz = this->tensor[5];
    float dxy = this->tensor[1];
    float dxz = this->tensor[2];
    float dyz = this->tensor[4];
    for(int i = 0; i < 3; i++) {
        float eigenvalue = this->eigenvalues[i];
        float a = dxx - eigenvalue;
        float b = dyy - eigenvalue;
        float c = dzz - eigenvalue;
        float e_x = (dxy*dyz - b*dxz)*(dxz*dyz - c*dxy);
        float e_y = (dxz*dyz - c*dxy)*(dxy*dxz - a*dyz);
        float e_z = (dxy*dxz - a*dyz)*(dxy*dyz - b*dxz);
        float magnitude = sqrt(e_x*e_x + e_y*e_y + e_z*e_z);
        switch(i) {
            case 0:
                this->eigenvector1[0] = e_x/magnitude;
                this->eigenvector1[1] = e_y/magnitude;
                this->eigenvector1[2] = e_z/magnitude;
                break;
            case 1:
                this->eigenvector2[0] = e_x/magnitude;
                this->eigenvector2[1] = e_y/magnitude;
                this->eigenvector2[2] = e_z/magnitude;
                break;
            case 2:
                this->eigenvector3[0] = e_x/magnitude;
                this->eigenvector3[1] = e_y/magnitude;
                this->eigenvector3[2] = e_z/magnitude;
                break;
        }
    }
}

float * Tensor3DProcessor::getEigenValues() {
    return this->eigenvalues;
}

float * Tensor3DProcessor::getEigenvector(int index) {
    if(index == 0) return this->eigenvector1;
    else if(index == 1) return this->eigenvector2;
    else return this->eigenvector3;
}

TensorFactory::TensorFactory(pbge::GraphicAPI * _gfx) {
    this->gfx = _gfx;
	this->ellipsoids = new Ellipsoids(_gfx);
	this->numberOfTensorsIsSet = true;
}

void TensorFactory::createTensors(unsigned n, float _scale_factor, float _max_entry) {
	this->ellipsoids->createEllipsoids(n);
	this->numberOfTensorsIsSet = true;
	this->scale_factor = _scale_factor;
	this->max_entry = _max_entry;
}

float mean(float a, float b, float c) {
	return (a+b+c)/3.0f;
}

void TensorFactory::addTensor(float *tensor, int order, int slices, const math3d::matrix44 & transformation) {
	if(!this->numberOfTensorsIsSet) throw std::exception("Number of tensors to be created is not set. Call createTensors first");
	if(order == 3) {
        Tensor3DProcessor * processor = new Tensor3DProcessor(tensor);
        float * eigenvalues = processor->getEigenValues();
        float * eigenvectors[3];
        for(int i = 0; i < 3; i++) {
            eigenvectors[i] = processor->getEigenvector(i);
        }
        math3d::matrix44 * rotation = new math3d::matrix44(eigenvectors[0][0], eigenvectors[1][0], eigenvectors[2][0], 0,
                                                           eigenvectors[0][1], eigenvectors[1][1], eigenvectors[2][1], 0,
                                                           eigenvectors[0][2], eigenvectors[1][2], eigenvectors[2][2], 0,
                                                           0, 0, 0, 1);
		math3d::matrix44 scale = math3d::scaleMatrix(eigenvalues[1] * this->scale_factor, eigenvalues[2] * this->scale_factor, eigenvalues[0] * this->scale_factor);
		//printf("%f %f %f\n\n", eigenvalues[0], eigenvalues[1], eigenvalues[2]);
		math3d::matrix44 transform = transformation * (*rotation) * scale;
		this->ellipsoids->addTransform(transform, mean(eigenvalues[0], eigenvalues[1], eigenvalues[2])/this->max_entry);
		//printf("%f\n", mean(eigenvalues[0], eigenvalues[1], eigenvalues[2])/max_entry);
		//math3d::matrix44 scale = math3d::scaleMatrix(0.1, 0.1, 0.1);
		//this->ellipsoids->addTransform(transformation*scale);
    }
}

pbge::ModelCollection * TensorFactory::done() {
	return this->ellipsoids->done(this->gfx);
}