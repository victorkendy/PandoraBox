#include <iostream>
#include <fstream>
#include <exception>

#include "FieldReader.h"

#include "pbge/pbge.h"
#include "TensorModel.h"
#include "TensorFactory.h"

#include "vtkmsqAnalyzeHeader.h"

Symmetric3DTensorFieldReader::Symmetric3DTensorFieldReader(pbge::GraphicAPI * gfx) {
    this->tensorFactory = new TensorFactory(gfx);
}

void Symmetric3DTensorFieldReader::loadField(const std::string & filename) {
    this->description_file.open(filename.c_str(), std::ifstream::in);
    this->description_file >> this->n_x;
    this->description_file >> this->n_y;
    this->description_file >> this->n_z;
    this->description_file >> this->d_x;
    this->description_file >> this->d_y;
    this->description_file >> this->d_z;
    if(this->n_x <= 0 || this->n_y <= 0 || this->n_z <= 0 ) throw pbge::IllegalArgumentException("Field dimensions must be positive");
    if(this->d_x <= 0 || this->d_y <= 0 || this->d_z <= 0 ) throw pbge::IllegalArgumentException("Field distances must be positive");
    this->read_field();
}

void Symmetric3DTensorFieldReader::read_field_parameters() {
    // TODO: Refactor this ASAP
    this->field = (float *****)malloc(this->n_x * sizeof(float ****));
    for(int i = 0; i < this->n_x; i++) {
        this->field[i] = (float ****)malloc(this->n_y * sizeof(float ***));
        for(int j = 0; j < this->n_y; j++) {
            this->field[i][j] = (float ***)malloc(this->n_z * sizeof(float **));
            for(int k = 0; k < this->n_z; k++) {
                this->field[i][j][k] = (float **)malloc(3 * sizeof(float *));
                for(int l = 0; l < 3; l++) {
                    this->field[i][j][k][l] = (float *)malloc(3 * sizeof(float));
                }
            }
        }
    }
}

void Symmetric3DTensorFieldReader::read_field() {
    this->read_field_parameters();
    for(int entry_count = 0; entry_count < 6; entry_count++) {
        for(int k = 0; k < this->n_z; k++) {
            for(int j = 0; j < this->n_y; j++) {
                for(int i = 0; i < this->n_x; i++) {
                    float value;
                    this->description_file >> value;
                    if(entry_count == 0) this->field[i][j][k][0][0] = value;
                    else if(entry_count == 1) this->field[i][j][k][1][0] = this->field[i][j][k][0][1] = value;
                    else if(entry_count == 2) this->field[i][j][k][1][1] = value;
                    else if(entry_count == 3) this->field[i][j][k][2][0] = this->field[i][j][k][0][2] = value;
                    else if(entry_count == 4) this->field[i][j][k][2][1] = this->field[i][j][k][1][2] = value;
                    else if(entry_count == 5) this->field[i][j][k][2][2] = value;
                }
            }
        }
    }
}

void Symmetric3DTensorFieldReader::generateFieldOn(pbge::SceneGraph *scene, int index) {
    generateFieldOn(scene, scene->getGraphNode(index));
}

void Symmetric3DTensorFieldReader::generateFieldOn(pbge::SceneGraph *scene, pbge::Node *parent) {
	if(!this->description_file.is_open()) throw std::exception("No tensor field description file loaded");
    pbge::Node * field_parent = scene->appendChildTo(parent, pbge::TransformationNode::translation(0,0,0));
	this->tensorFactory->createTensors(this->n_x*this->n_y*this->n_z);
    for(int k = 0; k < this->n_z; k++) {
        //pbge::Node * z_axis = scene->appendChildTo(field_parent, pbge::TransformationNode::translation(0, 0, -(k * this->d_z)));
        math3d::matrix44 kTranslation = math3d::translationMatrix(0, 0, -(k * this->d_z));
		for(int j = 0; j < this->n_y; j++) {
            //pbge::Node * y_axis = scene->appendChildTo(z_axis, pbge::TransformationNode::translation(0, -(j * this->d_y), 0));
            math3d::matrix44 jTranslation = math3d::translationMatrix(0, -(j * this->d_y), 0);
			for(int i = 0; i < this->n_x; i++) {
                //pbge::Node * x_axis = scene->appendChildTo(y_axis, pbge::TransformationNode::translation(i * this->d_x, 0, 0));
                //scene->appendChildTo(x_axis, this->tensorFactory->addTensor(this->field[i][j][k], 3, 16));
                //scene->appendChildTo(x_axis, new pbge::ModelInstance(new TensorModel(this->field[i][j][k], 3, 10)));
				math3d::matrix44 iTranslation = math3d::translationMatrix(i * this->d_x, 0, 0);
				this->tensorFactory->addTensor(this->field[j][i][k], 3, 16, iTranslation*jTranslation*kTranslation);
            }
        }
    }
}

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

bool is_zero_matrix(float **matrix) {
    float epsilon = 0.001f;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
			if(matrix[i][j] > epsilon || matrix[i][j] < -epsilon) return false;
        }
    }
    return true;
}

void AnalyzeReader::generateFieldOn(pbge::SceneGraph *scene, pbge::Node *parent) {
	read_field_with_right_type();
    int count = 0;
	pbge::Node * field_parent = scene->appendChildTo(parent, pbge::TransformationNode::translation((float)-this->header.dime.dim[1]/2,(float)this->header.dime.dim[2]/2,(float)this->header.dime.dim[3]/2));
	this->tensorFactory->createTensors(this->header.dime.dim[1]*this->header.dime.dim[2]*this->header.dime.dim[3]);
	for(int k = 0; k < this->header.dime.dim[3]; k++) {
        //pbge::Node * z_axis = scene->appendChildTo(field_parent, pbge::TransformationNode::translation(0, 0, -(k * this->header.dime.pixdim[3])));
		math3d::matrix44 kTranslation = math3d::translationMatrix(0, 0, -(k * this->header.dime.pixdim[3]));
        for(int j = 0; j < this->header.dime.dim[2]; j++) {
            //pbge::Node * y_axis = scene->appendChildTo(z_axis, pbge::TransformationNode::translation(0, -(j * this->header.dime.pixdim[2]), 0));
			math3d::matrix44 jTranslation = math3d::translationMatrix(0, -(j * this->header.dime.pixdim[2]), 0);
            for(int i = 0; i < this->header.dime.dim[1]; i++) {
				if(!is_zero_matrix(this->field[i][j][k])) {
					//printf("%.10f %.10f %.10f\n%.10f %.10f %.10f\n%.10f %.10f %.10f\n\n", this->field[i][j][k][0][0], this->field[i][j][k][0][1], this->field[i][j][k][0][2], this->field[i][j][k][1][0], this->field[i][j][k][1][1], this->field[i][j][k][1][2], this->field[i][j][k][2][0], this->field[i][j][k][2][1], this->field[i][j][k][2][2]);
                    //pbge::Node * x_axis = scene->appendChildTo(y_axis, pbge::TransformationNode::translation(i * this->header.dime.pixdim[1], 0, 0));
                    //scene->appendChildTo(x_axis, this->tensorFactory->addTensor(this->field[j][i][k], 3, 16));
					math3d::matrix44 iTranslation = math3d::translationMatrix(i * this->header.dime.pixdim[1], 0, 0);
					math3d::matrix44 a = iTranslation*jTranslation*kTranslation;
					this->tensorFactory->addTensor(this->field[j][i][k], 3, 16, iTranslation*jTranslation*kTranslation);
                }
            }
        }
    }
	scene->appendChildTo(field_parent, this->tensorFactory->done());
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
	
    alloc_field();
    
    short int row_size = this->header.dime.dim[1];
    short int rows_per_slice = this->header.dime.dim[2];
    short int number_of_slices = this->header.dime.dim[3];
    int slice_size = row_size * rows_per_slice;
    T * slice_buffer;
    slice_buffer = (T*)malloc(slice_size * sizeof(T));
    int number_of_components = 6;
    
    description_file.open(img.c_str(), std::ifstream::in | std::ifstream::binary);
    for(int component = 0; component < number_of_components; component++) {
        for(int slice = 0; slice < number_of_slices; slice++) {
            description_file.read(reinterpret_cast<char *>(slice_buffer), slice_size*sizeof(T));
            for(int row = 0; row < rows_per_slice; row++) {
                for(int column = 0; column < row_size; column++) {
                    T entry = slice_buffer[row*row_size + column];
                    if(component == 0) this->field[column][row][slice][0][0] = (float) entry;
                    else if(component == 1) this->field[column][row][slice][1][0] = this->field[column][row][slice][0][1] = (float) entry;
                    else if(component == 2) this->field[column][row][slice][2][0] = this->field[column][row][slice][0][2] = (float) entry;
                    else if(component == 3) this->field[column][row][slice][1][1] = (float) entry;
                    else if(component == 4) this->field[column][row][slice][2][1] = this->field[column][row][slice][1][2] = (float) entry;
                    else if(component == 5) this->field[column][row][slice][2][2] = (float) entry;
                }
            }
        }
    }
    
    description_file.close();
    free(slice_buffer);
}

void AnalyzeReader::alloc_field() {
    // TODO: Refactor this ASAP
    this->field = (float *****)malloc(this->header.dime.dim[1] * sizeof(float ****));
    for(int i = 0; i < this->header.dime.dim[1]; i++) {
        this->field[i] = (float ****)malloc(this->header.dime.dim[2] * sizeof(float ***));
        for(int j = 0; j < this->header.dime.dim[2]; j++) {
            this->field[i][j] = (float ***)malloc(this->header.dime.dim[3] * sizeof(float **));
            for(int k = 0; k < this->header.dime.dim[3]; k++) {
                this->field[i][j][k] = (float **)malloc(3 * sizeof(float *));
                for(int l = 0; l < 3; l++) {
                    this->field[i][j][k][l] = (float *)malloc(3 * sizeof(float));
                }
            }
        }
    }
}