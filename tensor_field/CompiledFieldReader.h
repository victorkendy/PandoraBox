#ifndef TENSORFIELD_COMPILEDFIELDREADER_H
#define TENSORFIELD_COMPILEDFIELDREADER_H

#include <string>
#include <boost/smart_ptr/scoped_array.hpp>

#include "pbge/pbge.h"
#include "math3d/math3d.h"

class CompiledFieldReader {
public:
    void read(const std::string & filename);
    void generateFieldOn(pbge::Node * parent, pbge::GraphicAPI * gfx);
private:
    boost::scoped_array<math3d::matrix44> transforms;
    unsigned number_of_tensors;
    unsigned step_size;
};

#endif