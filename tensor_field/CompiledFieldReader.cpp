#include <cstdio>
#include <algorithm>

#include "CompiledFieldReader.h"
#include "Ellipsoids.h"

void CompiledFieldReader::read(const std::string & filename) {
    FILE * inputfile = fopen(filename.c_str(), "rb");

    if(inputfile == NULL) throw 1;
    fread(&number_of_tensors, sizeof(unsigned), 1, inputfile);
    fread(&step_size, sizeof(unsigned), 1, inputfile);
    transforms.reset(new math3d::matrix44[number_of_tensors]);
    fread(transforms.get(), sizeof(math3d::matrix44), number_of_tensors, inputfile);
}

void CompiledFieldReader::generateFieldOn(pbge::Node * parent, pbge::GraphicAPI * gfx) {
    Ellipsoids ellipsoids(gfx);
    for(unsigned i = 0; i < number_of_tensors; i += step_size) {
        unsigned size;
        if((int)number_of_tensors - (int)(i + step_size) >= 0) {
            size = step_size;
        }
        else {
            size = number_of_tensors - i;
        }
        pbge::ModelCollection * collection = ellipsoids.createEllipsoids(size, transforms.get() + i);
        parent->addChild(collection);
    }
}