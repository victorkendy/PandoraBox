#ifndef PBGE_FIELD_COMPILER_TENSORDATA_H
#define PBGE_FIELD_COMPILER_TENSORDATA_H

struct Field_Dimensions {
    Field_Dimensions(float * _pixdim, float _fielddim1, float _fielddim2, float _fielddim3) {
        pixdim[0] = _pixdim[1];
        pixdim[1] = _pixdim[2];
        pixdim[2] = _pixdim[3];
        fielddim[0] = _fielddim1;
        fielddim[1] = _fielddim2;
        fielddim[2] = _fielddim3;
    }
    float pixdim[3];
    float fielddim[3];
};

class TensorData {
public:
    TensorData(int _row, int _column, int _slice, Field_Dimensions _dim): 
                row(_row), column(_column), slice(_slice), dim(_dim) {}
	void setComponent(const int & component, const float & value) {this->values[component] = value;}
	const math3d::matrix44 getTranslationToPosition();
	float * getValues() {return values;}
	bool is_zero();
    int get_row() {return row;}
    int get_column() {return column;}
    int get_slice() {return slice;}
private:
	int row,column,slice;
    Field_Dimensions dim;
	float values[6];
};

#endif