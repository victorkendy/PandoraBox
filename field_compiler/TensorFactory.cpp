#include <cmath>
#include <algorithm>
#include <cfloat>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "TensorFactory.h"

#define TENSOR_FACTORY_PI 3.14159f
#define STEP_SIZE 5000

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

TensorFactory::TensorFactory(unsigned n, float _scale_factor, float _max_entry) : 
    numberOfTensorsIsSet(true),
    transforms(new math3d::matrix44[n]),
    scale_factor(_scale_factor),
    max_entry(_max_entry),
    last_position(0),
    min_alpha(FLT_MAX),
    max_alpha(0.0f) {}

float TensorFactory::calculateRoundedAlpha(float * eigenvalues) {
    float alpha = floor(100.0f * calculateAlpha(eigenvalues))/100.0f;
    if(alpha > max_alpha) max_alpha = alpha;
    if(alpha < min_alpha) min_alpha = alpha;
    return alpha;
}

bool dec(float a, float b) {return a > b;}

float TensorFactory::calculateAlpha(float * eigenvalues) {
    float e[3] = {abs(eigenvalues[0]),abs(eigenvalues[1]),abs(eigenvalues[2])};
    std::sort(e, e + 3, dec);
    
    return 1.0f - (3*e[2])/(e[0] + e[1] + e[2]);
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
		math3d::matrix44 transform = transformation * (*rotation) * scale;
        transform.setRow(3, math3d::vector4(eigenvectors[0][0], eigenvectors[0][1], eigenvectors[0][2], calculateRoundedAlpha(eigenvalues)));
        this->transforms[this->last_position++] = transform.transpose();
    }
}

void TensorFactory::done(const std::string & filename) {
    FILE * outputfile = fopen(filename.c_str(), "wb");
    unsigned step_size = STEP_SIZE;
    float alpha_step = std::max(floor((max_alpha - min_alpha) * 10.0f) / 100.0f, 0.01f);
    fwrite(&last_position, sizeof(unsigned), 1, outputfile);
    fwrite(&step_size, sizeof(unsigned), 1, outputfile);
    fwrite(&min_alpha, sizeof(float), 1, outputfile);
    fwrite(&max_alpha, sizeof(float), 1, outputfile);
    fwrite(&alpha_step, sizeof(float), 1, outputfile);
    write_sorted_transforms(outputfile);
    fclose(outputfile);
    printf("min_alpha: %f max_alpha: %f alpha_step: %f\n", min_alpha, max_alpha, alpha_step);
}

class Comparator {
public:
    Comparator(const math3d::matrix44 & _pivot) {
        this->pivot = math3d::vector4(_pivot[0][3], _pivot[1][3], _pivot[2][3]);
    }

    Comparator() {}

    bool operator()(const math3d::matrix44 & _first, const math3d::matrix44 & _second) {
        math3d::vector4 first(_first[0][3], _first[1][3], _first[2][3]);
        math3d::vector4 second(_second[0][3], _second[1][3], _second[2][3]);
        
        if((first - pivot).size() < (second - pivot).size()) {
            return true;
        }
        return false;
    }
private:
    math3d::vector4 pivot;
};

struct BoundingBox {
    float max_x, max_y, max_z;
    float min_x, min_y, min_z;
};

void add_block_bounding_box(math3d::matrix44 * block_first, math3d::matrix44 * block_last, boost::scoped_array<BoundingBox> & bounding_boxes, int * current_box) {
    BoundingBox * box = new BoundingBox;

    box->max_x = box->max_y = box->max_z = FLT_MIN;
    box->min_x = box->min_y = box->min_z = FLT_MAX;

    for(math3d::matrix44 * it = block_first; it < block_last; it++) {
        float x = (*it)[3][0], y = (*it)[3][1], z = (*it)[3][2];
        if(x > box->max_x) box->max_x = x;
        if(y > box->max_y) box->max_y = y;
        if(z > box->max_z) box->max_z = z;
        if(x < box->min_x) box->min_x = x;
        if(y < box->min_y) box->min_y = y;
        if(z < box->min_z) box->min_z = z;
    }

    bounding_boxes[(*current_box)++] = *box;
}

void TensorFactory::write_sorted_transforms(FILE * outputfile) {
    Comparator comparator;
    math3d::matrix44 * first = transforms.get();
    math3d::matrix44 * last = first + last_position;
    unsigned current_pos = 0;
    int current_box = 0;

    int number_of_boxes = static_cast<int>(ceil(last_position / (double) STEP_SIZE));
    boost::scoped_array<BoundingBox> boxes(new BoundingBox[number_of_boxes]);

    while(current_pos < last_position) {
        math3d::matrix44 * block_first = first;
        math3d::matrix44 * block_last = block_first + std::min((unsigned)STEP_SIZE, last_position - current_pos);
        comparator = Comparator(first[0]);
        std::sort(first, last, comparator);
        add_block_bounding_box(block_first, block_last, boxes, &current_box);
        current_pos += STEP_SIZE;
        first += STEP_SIZE;
    }
    
    fwrite(&number_of_boxes, sizeof(int), 1, outputfile);
    fwrite(boxes.get(), sizeof(BoundingBox), number_of_boxes, outputfile);
    fwrite(transforms.get(), sizeof(math3d::matrix44), last_position, outputfile);
}
