#ifndef PBGE_TENSOR_FIELD_FIELDREADER_H
#define PBGE_TENSOR_FIELD_FIELDREADER_H

#include <string>
#include <fstream>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "TensorFactory.h"

#include "vtkmsqAnalyzeHeader.h"

class TensorData {
public:
	TensorData(int _row, int _column, int _slice, float pixdim1, float pixdim2, float pixdim3): row(_row), column(_column), slice(_slice) {
		pixdim[0] = pixdim1;
		pixdim[1] = pixdim2;
		pixdim[2] = pixdim3;
	}
	void setComponent(const int & component, const float & value) {this->values[component] = value;}
	const math3d::matrix44 getTranslationToPosition();
	float * getValues() {return values;}
	bool is_zero();
private:
	int row,column,slice;
	float pixdim[3];
	float values[6];
};

class AnalyzeReader {
public:
    void loadField(const std::string & filename);
    void generateField(const std::string & outputfile);
private:
    std::string filename;
	std::vector<TensorData> tensors;
    struct analyze_dsr header;
    boost::scoped_ptr<TensorFactory> tensorFactory;
	float max_entry;
    
    template <class T> void read_field();
	void read_field_with_right_type();
    void build_tensor_vector();
	int index_of(int column, int row, int slice);
	void add_tensor(TensorData tensor);
};

#endif