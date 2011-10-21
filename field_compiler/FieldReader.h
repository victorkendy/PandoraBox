#ifndef PBGE_TENSOR_FIELD_FIELDREADER_H
#define PBGE_TENSOR_FIELD_FIELDREADER_H

#include <string>
#include <fstream>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "TensorFactory.h"
#include "TensorData.h"

#include "vtkmsqAnalyzeHeader.h"

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