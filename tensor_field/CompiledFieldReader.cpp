#include <cstdio>
#include <algorithm>

#include "CompiledFieldReader.h"
#include "Ellipsoids.h"

void CompiledFieldReader::read(const std::string & filename) {
    FILE * inputfile = fopen(filename.c_str(), "rb");

    if(inputfile == NULL) throw 1;
    fread(&number_of_tensors, sizeof(unsigned), 1, inputfile);
    fread(&step_size, sizeof(unsigned), 1, inputfile);
    fread(&number_of_boxes, sizeof(int), 1, inputfile);
    transforms.reset(new math3d::matrix44[number_of_tensors]);
    boxes.reset(new BoundingBox[number_of_boxes]);
    fread(boxes.get(), sizeof(BoundingBox), number_of_boxes, inputfile);
    fread(transforms.get(), sizeof(math3d::matrix44), number_of_tensors, inputfile);
}

void CompiledFieldReader::generateFieldOn(pbge::Node * parent, pbge::GraphicAPI * gfx) {
    Ellipsoids ellipsoids(gfx, this->number_of_tensors);
    int box = 0;
    for(unsigned i = 0; i < number_of_tensors && box < number_of_boxes; i += step_size, box++) {
        unsigned size;
        if((int)number_of_tensors - (int)(i + step_size) >= 0) {
            size = step_size;
        }
        else {
            size = number_of_tensors - i;
        }
        pbge::ModelCollection * collection = ellipsoids.createEllipsoids(size, transforms.get() + i, boxes[box]);
        parent->addChild(collection);
    }
}