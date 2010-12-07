

#ifndef gfxvbo_h_
#define gfxvbo_h_

#include <vector>

#include "pbge/core/Vec3.h"

namespace pbge {

// Vertex Buffer Object Interface
class VertexBuffer {
public:

    virtual void render() = 0;

    virtual void getDataPointer() = 0;

    virtual void begin() = 0;

    virtual void end() = 0;

};

class VertexBufferBuilder {
public:

    void push_vertex(const Vec3 & v) {}

    void set_vertex_index(const std::vector<int> & indexes) {}

    void done() {}

    float * get_data() { return new float [100]; }
};

}

#endif