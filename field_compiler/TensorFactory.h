#ifndef PBGE_TENSOR_FIELD_TENSORFACTORY_H_
#define PBGE_TENSOR_FIELD_TENSORFACTORY_H_

#include <vector>
#include <set>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <functional>
#include <cfloat>
#include <string>
#include <boost/smart_ptr/scoped_array.hpp>

#include "math3d/math3d.h"
#include "TensorData.h"

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

class BBTransformation {
public:
    BBTransformation(const math3d::matrix44 & transform) {
        memcpy(transformation, transform, 16 * sizeof(float));
    }
    bool operator < (const BBTransformation & other) const {
        return other.transformation[15] < transformation[15];
    }
    void write(FILE * file) {
        fwrite(transformation, sizeof(float), 16, file);
    }
private:
    float transformation[16];
};

struct BoundingBox {
    float max_coord[3];
    float min_coord[3];

    BoundingBox() {
        max_coord[0] = max_coord[1] = max_coord[2] = -FLT_MAX;
        min_coord[0] = min_coord[1] = min_coord[2] = FLT_MAX;
    }
    std::multiset<BBTransformation> transformations;
    bool should_write() {
        return !transformations.empty();
    }
    void write_bouding_box(FILE * file) {
        if(should_write()) {
            fwrite(max_coord, sizeof(float), 3, file);
            fwrite(min_coord, sizeof(float), 3, file);
        }
    }
    void write_transformation_count(FILE * file) {
        if(should_write()) {
            int transform_count = transformations.size();
            fwrite(&transform_count, sizeof(int), 1, file);
        }
    }
    void write_transformations(FILE * file) {
        std::for_each(transformations.begin(), transformations.end(), std::bind2nd(std::mem_fun_ref(&BBTransformation::write), file));
    }
    void insert(const math3d::matrix44 & transform) {
        transformations.insert(transform);
        
        for(int i = 0; i < 3; i++) {
            if(transform[3][i] > max_coord[i]) max_coord[i] = transform[3][i];
            if(transform[3][i] < min_coord[i]) min_coord[i] = transform[3][i];
        }
    }
};

class TensorFactory {
public:
    TensorFactory(unsigned n, float _scale_factor, float _max_entry, int row, int column, int slice, float pix_dim[4]);

	// slices is not being used yet!
    void addTensor(TensorData & tensor, int order, int slices);
    void done(const std::string & filename);
private:
    std::vector<BoundingBox> boxes;
	bool numberOfTensorsIsSet;
	float scale_factor;
	float max_entry;
    float min_alpha;
    float max_alpha;
    int number_of_transforms;
    int rows, columns, slices;
    float dim[3];

    void write_transforms(FILE * outputfile);
    float calculateRoundedAlpha(float * eigenvalues);
    float calculateAlpha(float * eigenvalues);
};

#endif
