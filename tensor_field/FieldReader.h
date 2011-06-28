#ifndef PBGE_TENSOR_FIELD_FIELDREADER_H
#define PBGE_TENSOR_FIELD_FIELDREADER_H

#include <string>
#include <fstream>

#include "pbge/pbge.h"
#include "TensorFactory.h"

#include "vtkmsqAnalyzeHeader.h"

class FieldReader {
public:
    virtual void loadField(const std::string & filename)=0;
    virtual void generateFieldOn(pbge::SceneGraph * scene, pbge::Node * parent)=0;
    virtual void generateFieldOn(pbge::SceneGraph * scene, int index)=0;
};

class Symmetric3DTensorFieldReader : public FieldReader {
/**
Expected format:
n_x n_y n_z
d_x d_y d_z
a1_00 a2_00 a3_00 ... a(n_x*n_y*n_z)_00
a1_10 a2_10 a3_10 ... a(n_x*n_y*n_z)_10
a1_11 a2_11 a3_11 ... a(n_x*n_y*n_z)_11
a1_20 a2_20 a3_20 ... a(n_x*n_y*n_z)_20
a1_21 a2_21 a3_21 ... a(n_x*n_y*n_z)_21
a1_22 a2_22 a3_22 ... a(n_x*n_y*n_z)_22

Where:
- n_i is the number of tensors in the i-axis
- d_j is the distance between each tensor in the j-axis
- each tensor ak is:
    ak_00 ak_10 ak_20
    ak_10 ak_11 ak_21
    ak_20 ak_21 ak_22

Tensors are supposed to be ordered from top to bottom and front to back.
Note that each tensor entry is given separately (first the first entry of all tensors, then the second entry, etc.
*/
public:
    Symmetric3DTensorFieldReader(pbge::GraphicAPI * gfx);
    void loadField(const std::string & filename);
    void generateFieldOn(pbge::SceneGraph * scene, pbge::Node * parent);
    void generateFieldOn(pbge::SceneGraph * scene, int index);
private:
    std::ifstream description_file;
    int n_x, n_y, n_z;
    float d_x, d_y, d_z;
    float *****field;
    TensorFactory * tensorFactory;

    void read_field_parameters();
    void read_field();
};

class AnalyzeReader : public FieldReader {
public:
    AnalyzeReader(pbge::GraphicAPI * gfx);
    void loadField(const std::string & filename);
    void generateFieldOn(pbge::SceneGraph * scene, pbge::Node * parent);
    void generateFieldOn(pbge::SceneGraph * scene, int index);
private:
    std::string filename;
    float *****field;
    struct analyze_dsr header;
    TensorFactory * tensorFactory;
    
    template <class T> void read_field();
    void read_field_with_right_type();
    void alloc_field();
};

#endif