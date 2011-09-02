#ifndef PBGE_TENSOR_FIELD_TENSORFACTORY_H_
#define PBGE_TENSOR_FIELD_TENSORFACTORY_H_

#include <vector>
#include <string>
#include <boost/smart_ptr/scoped_array.hpp>

#include "math3d/math3d.h"

class Tensor3DProcessor {
public:
    Tensor3DProcessor(float * tensor);

    float * getEigenValues();
    float * getEigenvector(int index);

private:
    float * tensor;
    float eigenvalues[3];
    float eigenvector1[3];
    float eigenvector2[3];
    float eigenvector3[3];

    bool isTensorDiagonal();
    void calculateEigenValues();
    void calculateEigenVectors();
    void calculateEigenValuesForDiagonalTensor();
    void calculateEigenValuesForNonDiagonalTensor();
    void calculateEigenVectorsForDiagonalTensor();
    void calculateEigenVectorsForNonDiagonalTensor();
};

class TensorFactory {
public:
    TensorFactory(unsigned n, float _scale_factor, float _max_entry);

	// slices is not being used yet!
    void addTensor(float * tensor, int order, int slices, const math3d::matrix44 & transformation);
    void done(const std::string & filename);
private:
    boost::scoped_array<math3d::matrix44> transforms;
	bool numberOfTensorsIsSet;
	float scale_factor;
	float max_entry;
    unsigned last_position;
};

#endif
