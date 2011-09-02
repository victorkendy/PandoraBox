#include <cstdio>

#include "CompiledFieldReader.h"
#include "Ellipsoids.h"

void CompiledFieldReader::read(const std::string & filename) {
    FILE * inputfile = fopen(filename.c_str(), "rb");

    if(inputfile == NULL) throw 1;
    fread(&number_of_tensors, sizeof(unsigned), 1, inputfile);
    transforms.reset(new math3d::matrix44[number_of_tensors]);
    fread(transforms.get(), sizeof(math3d::matrix44), number_of_tensors, inputfile);
}

void CompiledFieldReader::generateFieldOn(pbge::Node * parent, pbge::GraphicAPI * gfx) {
    Ellipsoids ellipsoids(gfx);
    pbge::ModelCollection * collection = ellipsoids.createEllipsoids(number_of_tensors, transforms.get());
    parent->addChild(collection);
}