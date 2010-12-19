

#ifndef gfxvbo_h_
#define gfxvbo_h_

#include <vector>

#include "pbge/core/core.h"
#include "pbge/core/Vec3.h"

namespace pbge {

    class PBGE_EXPORT VertexBufferBuilder {
    public:
        VertexBufferBuilder(int _vertex_dim, int _normal_dim = 0) {
            data = NULL;
            normal_dim = _normal_dim;
            vertex_dim = _vertex_dim;
        }

        ~VertexBufferBuilder() {
            if(data != NULL)
                delete [] data;
        }

        VertexBufferBuilder * push_vertex(const float x, const float y=0.0f, const float z=0.0f, const float w=1.0f) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(w);
            return this;
        }

        void push_normal(const float x, const float y=0.0f, const float z=0.0f, const float w=1.0f) {
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
            normals.push_back(w);
        }

        void set_vertex_index(const std::vector<unsigned> & indexes) {
            vertex_indexes = indexes;
        }

        void set_normal_index(const std::vector<unsigned> & indexes) {
            normal_indexes = indexes;
        }

        void done();

        float * get_data() { 
            return data; 
        }

    private:
        int vertex_dim, normal_dim;

        std::vector<float> vertices;

        std::vector<unsigned> vertex_indexes;

        std::vector<float> normals;

        std::vector<unsigned> normal_indexes;

        float * data;
    };

    // Vertex Buffer Object Interface
    class VertexBuffer {
    public:

        virtual void render() = 0;

        virtual void getDataPointer() = 0;

        virtual void begin() = 0;

        virtual void end() = 0;

    };
}

#endif