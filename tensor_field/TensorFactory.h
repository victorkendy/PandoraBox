#ifndef PBGE_TENSOR_FIELD_TENSORFACTORY_H_
#define PBGE_TENSOR_FIELD_TENSORFACTORY_H_

#include "pbge/pbge.h"
#include "Ellipsoids.h"

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
    TensorFactory(pbge::GraphicAPI * gfx);

	void createTensors(unsigned n);
    // slices is not being used yet!
    void addTensor(float * tensor, int order, int slices, const math3d::matrix44 & transformation);
	pbge::ModelCollection * done();
private:
    pbge::GraphicAPI * gfx;
	Ellipsoids * ellipsoids;
	bool numberOfTensorsIsSet;
};

#endif
