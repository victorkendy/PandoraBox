#ifndef TENSOR_FIELD_LODMODELS_H_
#define TENSOR_FIELD_LODMODELS_H_

#include "pbge/pbge.h"

class LODModels {
public:
    LODModels(pbge::Model ** _models, float * _distances, int _size) : models(_models), distances(_distances), size(_size) {}
    pbge::Model * forDistance(float dist) {
        printf("%f\n", dist);
        if(size < 1) return NULL;
        if(dist < distances[0]) {
            return models[0];
        }
        for(int i = 1; i < size - 1; i++) {
            if(distances[i - 1] <= dist && dist < distances[i]) {
                return models[i];
            }
        }
        return models[size - 1];
    }
private:
    pbge::Model ** models;
    float * distances;
    int size;
};

#endif