#ifndef PBGE_TENSOR_FIELD_FIELDREADER_H
#define PBGE_TENSOR_FIELD_FIELDREADER_H

#include <string>
#include <fstream>

#include "pbge/pbge.h"
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

class FieldReader {
public:
    virtual void loadField(const std::string & filename)=0;
    virtual void generateFieldOn(pbge::SceneGraph * scene, pbge::Node * parent)=0;
    virtual void generateFieldOn(pbge::SceneGraph * scene, int index)=0;
};

class AnalyzeReader : public FieldReader {
public:
    AnalyzeReader(pbge::GraphicAPI * gfx);
    void loadField(const std::string & filename);
    void generateFieldOn(pbge::SceneGraph * scene, pbge::Node * parent);
    void generateFieldOn(pbge::SceneGraph * scene, int index);
private:
    std::string filename;
	std::vector<TensorData> tensors;
    struct analyze_dsr header;
    TensorFactory * tensorFactory;
	float max_entry;
    
    template <class T> void read_field();
	void read_field_with_right_type();
    void build_tensor_vector();
	int index_of(int column, int row, int slice);
	void add_tensor(TensorData tensor);
};

#endif