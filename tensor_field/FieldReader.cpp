#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <cmath>
#include <typeinfo>

#include "FieldReader.h"

#include "pbge/pbge.h"
#include "TensorModel.h"
#include "TensorFactory.h"

#include "vtkmsqAnalyzeHeader.h"

AnalyzeReader::AnalyzeReader(pbge::GraphicAPI * gfx) {
    this->tensorFactory = new TensorFactory(gfx);
}

void AnalyzeReader::loadField(const std::string & _filename) {
    std::ifstream description_file;
    this->filename = _filename;
    std::string hdr = _filename + ".hdr";
    description_file.open(hdr.c_str(), std::ifstream::in | std::ifstream::binary);
    description_file.read(reinterpret_cast<char *>(&this->header), sizeof(struct analyze_dsr));
    description_file.close();
}

void AnalyzeReader::generateFieldOn(pbge::SceneGraph *scene, int index) {
    generateFieldOn(scene, scene->getGraphNode(index));
}

void AnalyzeReader::generateFieldOn(pbge::SceneGraph *scene, pbge::Node *parent) {
	read_field_with_right_type();
    int count = 0;
	pbge::Node * field_parent = scene->appendChildTo(parent, pbge::TransformationNode::translation((float)-this->header.dime.dim[1]/2,(float)this->header.dime.dim[2]/2,(float)this->header.dime.dim[3]/2));
	this->tensorFactory->createTensors(this->header.dime.dim[1]*this->header.dime.dim[2]*this->header.dime.dim[3], std::max(std::max(this->header.dime.pixdim[0], this->header.dime.pixdim[1]), this->header.dime.pixdim[2])/this->max_entry);
	std::for_each(this->tensors.begin(), this->tensors.end(), std::bind1st(std::mem_fun(&AnalyzeReader::add_tensor), this));
	scene->appendChildTo(field_parent, this->tensorFactory->done());
}

void AnalyzeReader::add_tensor(TensorData tensor) {
	if(!tensor.is_zero()){
		//printf("%f %f %f %f %f %f\n\n", tensor.getValues()[0], tensor.getValues()[1], tensor.getValues()[2], tensor.getValues()[3], tensor.getValues()[4], tensor.getValues()[5]);
		this->tensorFactory->addTensor(tensor.getValues(), 3, 16, tensor.getTranslationToPosition());
	}
}

void AnalyzeReader::read_field_with_right_type() {
    switch(this->header.dime.datatype) {
        case ANALYZE_DT_UNKNOWN:
            throw std::exception("Unknown data type\n");
            break;
        case ANALYZE_DT_BINARY:
            read_field<bool>();
            break;
        case ANALYZE_DT_UNSIGNED_CHAR:
            read_field<unsigned char>();
            break;
        case ANALYZE_DT_SIGNED_SHORT:
            read_field<signed char>();
            break;
        case ANALYZE_DT_SIGNED_INT:
            read_field<signed int>();
            break;
        case ANALYZE_DT_FLOAT:
            read_field<float>();
            break;
        case ANALYZE_DT_COMPLEX:
            throw std::exception("Complex is not implemented yet\n");
            break;
        case ANALYZE_DT_DOUBLE:
            read_field<double>();
            break;
        case ANALYZE_DT_RGB:
            throw std::exception("RGB is not implemented yet\n");
            break;
        case ANALYZE_DT_ALL:
            throw std::exception("Unknown data type\n");
            break;
        case SPMANALYZE_DT_UNSIGNED_SHORT:
            read_field<unsigned short>();
            break;
        case SPMANALYZE_DT_UNSIGNED_INT:
            read_field<unsigned int>();
            break;
        default:
            break;
    }
}

template <class T> void AnalyzeReader::read_field() {
    std::ifstream description_file;
    std::string img = this->filename + ".img";
	
	build_tensor_vector();
    
    short int row_size = this->header.dime.dim[1];
    short int rows_per_slice = this->header.dime.dim[2];
    short int number_of_slices = this->header.dime.dim[3];
    int slice_size = row_size * rows_per_slice;
    T * slice_buffer;
	slice_buffer = (T*)malloc(slice_size * sizeof(T));
    int number_of_components = 6;
	float abs_val;

    description_file.open(img.c_str(), std::ifstream::in | std::ifstream::binary);
	for(int component = 0; component < number_of_components; component++) {
        for(int slice = 0; slice < number_of_slices; slice++) {
            description_file.read(reinterpret_cast<char *>(slice_buffer), slice_size*sizeof(T));
            for(int row = 0; row < rows_per_slice; row++) {
                for(int column = 0; column < row_size; column++) {
                    T entry = slice_buffer[row*row_size + column];
					this->tensors[index_of(column, row, slice)].setComponent(component, (float)entry);
					abs_val = std::abs((float)entry);
					if(abs_val > this->max_entry) this->max_entry = abs_val;
                }
            }
        }
    }
	
    description_file.close();
    free(slice_buffer);
}

void AnalyzeReader::build_tensor_vector() {
	for(int slice = 0; slice < this->header.dime.dim[3]; slice++) {
        for(int row = 0; row < this->header.dime.dim[2]; row++) {
            for(int column = 0; column < this->header.dime.dim[1]; column++) {
				tensors.push_back(TensorData(row, column, slice, this->header.dime.pixdim[1], this->header.dime.pixdim[2], this->header.dime.pixdim[3])); 
            }
        }
    }
}

int AnalyzeReader::index_of(int column, int row, int slice) {
	return column + this->header.dime.dim[1]*row + this->header.dime.dim[1]*this->header.dime.dim[2]*slice;
}

const math3d::matrix44 TensorData::getTranslationToPosition() {
	math3d::matrix44 sliceTranslation = math3d::translationMatrix(0, 0, -(slice * pixdim[2]));
	math3d::matrix44 rowTranslation = math3d::translationMatrix(0, -(row * pixdim[1]), 0);
	math3d::matrix44 columnTranslation = math3d::translationMatrix(column * pixdim[0], 0, 0);
	return rowTranslation*columnTranslation*sliceTranslation;
}

bool TensorData::is_zero() {
	//if(this->values[0] > 0.5) return false;
	//else return true;
	float epsilon = 0.001f;
    for(int i = 0; i < 6; i++) {
		if(this->values[i] > epsilon || this->values[i] < -epsilon) return false;
    }
    return true;
}