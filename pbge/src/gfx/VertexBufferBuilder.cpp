
#include "pbge/gfx/VBO.h"

namespace {
    inline void copy_data(float * destination, int dim, int index, const std::vector<float> & source, int & data_index) {
        if(index >= 0) {
            if((source.size() / 4) > static_cast<unsigned>(index)) {
                int begining_index = 4 * index;
                for(int i = 0; i < dim; i++)
                    destination[data_index++] = source[begining_index + i];
            } else
                throw 1;
        }
    }
}

namespace pbge {

    void VertexBufferBuilder::done() {
        if(data != NULL)
            delete [] data;
        data = new float[(normal_indexes.size() + vertex_indexes.size()) * (vertex_dim + normal_dim)];
        int data_index = 0;
        
        for(unsigned i = 0; i < vertex_indexes.size(); i++) {
            int v_index = vertex_indexes[i];
            int n_index = normal_dim > 0 ? normal_indexes[i]:-1;
            
            copy_data(data, vertex_dim, v_index, vertices, data_index);
            copy_data(data, normal_dim, n_index, normals, data_index);
        }
    }

}