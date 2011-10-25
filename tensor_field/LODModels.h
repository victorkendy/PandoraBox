#ifndef TENSOR_FIELD_LODMODELS_H_
#define TENSOR_FIELD_LODMODELS_H_

#include <vector>

#include "pbge/pbge.h"

class LODModels {
public:
    LODModels(std::vector<pbge::Model*> _models, std::vector<float> _distances) : models(_models), distances(_distances) {}
    pbge::Model * forDistance(float dist) {
        if(distances.empty()) return NULL;
        if(dist < distances[0]) {
            return models[0];
        }
        for(unsigned int i = 1; i < distances.size() - 1; i++) {
            if(distances[i - 1] <= dist && dist < distances[i]) {
                return models[i];
            }
        }
        return models[distances.size() - 1];
    }
private:
    std::vector<pbge::Model*> models;
    std::vector<float> distances;
};

#endif