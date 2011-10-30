#include <cmath>
#include <algorithm>
#include <cfloat>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "TensorFactory.h"
#include "Comparators.h"

#define TENSOR_FACTORY_PI 3.14159f
#define STEP_SIZE 10

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

TensorFactory::TensorFactory(unsigned n, float _scale_factor, float _max_entry, int row, int column, int slice, float pix_dim[4]) : 
    numberOfTensorsIsSet(true),
    scale_factor(_scale_factor),
    max_entry(_max_entry),
    number_of_transforms(0),
    min_alpha(FLT_MAX),
    max_alpha(0.0f) {
        rows = row/STEP_SIZE + 1;
        columns = column/STEP_SIZE + 1;
        slices = slice/STEP_SIZE + 1;
        boxes = std::vector<BoundingBox>(rows*columns*slices);
        dim[0] = pix_dim[1] * column;
        dim[1] = pix_dim[2] * row;
        dim[2] = pix_dim[3] * slice;
    }

math3d::vector4 TensorFactory::calculateRoundedAlphas(float * eigenvalues) {
    float e[3] = {abs(eigenvalues[0]),abs(eigenvalues[1]),abs(eigenvalues[2])};
    std::sort(e, e + 3, dec);
    float linear = roundAlpha(calculateAlphaLinear(e));
    float planar = roundAlpha(calculateAlphaPlanar(e));
    float spherical = roundAlpha(calculateAlphaSpherical(e));
    float anisotropy = roundAlpha(calculateAlphaAnisotropy(e));
    return math3d::vector4(linear, planar, spherical, anisotropy);
}

float TensorFactory::roundAlpha(float alpha) {
    alpha = floor(100.0f * alpha)/100.0f;
    if(alpha > max_alpha) max_alpha = alpha;
    if(alpha < min_alpha) min_alpha = alpha;
    return alpha;
}

float TensorFactory::calculateAlphaLinear(float * e) {
    return (e[0] - e[1])/(e[0] + e[1] + e[2]);
}

float TensorFactory::calculateAlphaPlanar(float * e) {
    return (2*(e[1] - e[2]))/(e[0] + e[1] + e[2]);
}

float TensorFactory::calculateAlphaSpherical(float * e) {
    return (3*e[2])/(e[0] + e[1] + e[2]);
}

float TensorFactory::calculateAlphaAnisotropy(float * e) {
    float den = sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    return sqrt(0.5f)*sqrt((e[0] - e[1])*(e[0] - e[1]) + (e[1] - e[2])*(e[1] - e[2]) + (e[0] - e[2])*(e[0] - e[2]))/den;
}

void TensorFactory::addTensor(TensorData & tensor, int order, int slices) {
	if(!this->numberOfTensorsIsSet) throw std::exception("Number of tensors to be created is not set. Call createTensors first");
	if(order == 3) {
        Tensor3DProcessor processor(tensor.getValues());
        float * eigenvalues = processor.getEigenValues();
        float * eigenvectors[3];
        for(int i = 0; i < 3; i++) {
            eigenvectors[i] = processor.getEigenvector(i);
        }
        math3d::matrix44 rotation(eigenvectors[0][0], eigenvectors[1][0], eigenvectors[2][0], 0,
                                  eigenvectors[0][1], eigenvectors[1][1], eigenvectors[2][1], 0,
                                  eigenvectors[0][2], eigenvectors[1][2], eigenvectors[2][2], 0,
                                  0, 0, 0, 1);
		math3d::matrix44 scale = math3d::scaleMatrix(eigenvalues[1] * this->scale_factor, eigenvalues[2] * this->scale_factor, eigenvalues[0] * this->scale_factor);
        math3d::matrix44 transform = tensor.getTranslationToPosition() * rotation * scale;
        transform.setRow(3, calculateRoundedAlphas(eigenvalues));
        int row_index = tensor.get_row()/STEP_SIZE;
        int column_index = tensor.get_column()/STEP_SIZE;
        int slice_index = tensor.get_slice()/STEP_SIZE;
        this->boxes[column_index + row_index*columns + slice_index*columns*rows].insert(transform.transpose());
        number_of_transforms++;
    }
}

void TensorFactory::done(const std::string & filename) {
    FILE * outputfile = fopen(filename.c_str(), "wb");
    float alpha_step = std::max(floor((max_alpha - min_alpha) * 10.0f) / 1000.0f, 0.01f);
    
    fwrite(&number_of_transforms, sizeof(int), 1, outputfile);
    fwrite(dim, sizeof(float), 3, outputfile);
    fwrite(&min_alpha, sizeof(float), 1, outputfile);
    fwrite(&max_alpha, sizeof(float), 1, outputfile);
    fwrite(&alpha_step, sizeof(float), 1, outputfile);
    write_transforms(outputfile);
    fclose(outputfile);
}

void TensorFactory::write_transforms(FILE * outputfile) {
    int number_of_boxes = std::count_if(boxes.begin(),boxes.end(),std::mem_fun_ref(&BoundingBox::should_write));    
    fwrite(&number_of_boxes, sizeof(int), 1, outputfile);
    std::for_each(boxes.begin(), boxes.end(), std::bind2nd(std::mem_fun_ref(&BoundingBox::write_transformation_count),outputfile));
    std::for_each(boxes.begin(), boxes.end(), std::bind2nd(std::mem_fun_ref(&BoundingBox::write_bouding_box),outputfile));
    std::for_each(boxes.begin(), boxes.end(), std::bind2nd(std::mem_fun_ref(&BoundingBox::write_transformations),outputfile));
}
