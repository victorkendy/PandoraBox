#include <cstdio>
#include <algorithm>

#include "CompiledFieldReader.h"
#include "Ellipsoids.h"

void CompiledFieldReader::read(const std::string & filename) {
    FILE * inputfile = fopen(filename.c_str(), "rb");

    if(inputfile == NULL) throw 1;
    fread(&number_of_tensors, sizeof(unsigned), 1, inputfile);
    fread(&step_size, sizeof(unsigned), 1, inputfile);
    fread(&min_alpha, sizeof(float), 1, inputfile);
    fread(&max_alpha, sizeof(float), 1, inputfile);
    fread(&alpha_step, sizeof(float), 1, inputfile);
    fread(&number_of_boxes, sizeof(int), 1, inputfile);
    transforms.reset(new math3d::matrix44[number_of_tensors]);
    boxes.reset(new BoundingBox[number_of_boxes]);
    fread(boxes.get(), sizeof(BoundingBox), number_of_boxes, inputfile);
    fread(transforms.get(), sizeof(math3d::matrix44), number_of_tensors, inputfile);
}

FieldParent * CompiledFieldReader::generateField(pbge::GraphicAPI * gfx) {
    Ellipsoids ellipsoids(gfx, this->number_of_tensors);
    int box = 0;
    FieldParent * parent = new FieldParent(ellipsoids.get_peeling_program(), min_alpha, max_alpha, alpha_step);
    for(unsigned i = 0; i < number_of_tensors && box < number_of_boxes; i += step_size, box++) {
        unsigned size;
        if((int)number_of_tensors - (int)(i + step_size) >= 0) {
            size = step_size;
        }
        else {
            size = number_of_tensors - i;
        }
        pbge::ModelCollection * collection = ellipsoids.createEllipsoids(size, transforms.get() + i, &(boxes[box]));
        printf("BOX max x:%f y:%f z:%f min x:%f y:%f z:%f\n", boxes[box].max_x, boxes[box].max_y, boxes[box].max_z, boxes[box].min_x, boxes[box].min_y, boxes[box].min_z);
        parent->addChild(collection);
    }
    printf("\n");
    return parent;
}