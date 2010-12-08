

#ifndef gfxvbo_h_
#define gfxvbo_h_

#include <vector>

#include "pbge/core/core.h"
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

    class PBGE_EXPORT VertexBufferBuilder {
    public:
        VertexBufferBuilder() {
            data = NULL;
        }

        ~VertexBufferBuilder() {
            if(data != NULL)
                delete [] data;
        }

        void push_vertex(const Vec3 & v) {
            vertices.push_back(v);
        }

        void set_vertex_index(const std::vector<unsigned> & indexes) {
            vertex_indexes = indexes;
        }

        void done();

        float * get_data() { 
            return data; 
        }

    private:

        std::vector<Vec3> vertices;

        std::vector<unsigned> vertex_indexes;

        float * data;
    };

}

#endif