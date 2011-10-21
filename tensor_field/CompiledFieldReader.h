#ifndef TENSORFIELD_COMPILEDFIELDREADER_H
#define TENSORFIELD_COMPILEDFIELDREADER_H

#include <string>
#include <boost/smart_ptr/scoped_array.hpp>

#include "pbge/pbge.h"
#include "math3d/math3d.h"
#include "BoundingBox.h"
#include "PeelingAwareNode.h"

class CompiledFieldReader {
public:
    void read(const std::string & filename);
    FieldParent * generateField(pbge::GraphicAPI * gfx);
private:
    boost::scoped_array<math3d::matrix44> transforms;
    int number_of_tensors;
    int number_of_boxes;
    boost::scoped_array<BoundingBox> boxes;
    boost::scoped_array<int> transforms_per_box;
    unsigned step_size;
    float min_alpha;
    float max_alpha;
    float alpha_step;
};

#endif