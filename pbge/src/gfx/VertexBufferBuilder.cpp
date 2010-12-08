
#include "pbge/gfx/VBO.h"


namespace pbge {

    void VertexBufferBuilder::done() {
        if(data != NULL)
            delete [] data;
        data = new float[this->vertex_indexes.size() * 4];
        int data_index = 0;
        std::vector<unsigned>::iterator it;
        for(it = vertex_indexes.begin(); it != vertex_indexes.end(); it++) {
            if(vertices.size() > *it) {
                Vec3 v = vertices[*it];
                data[data_index++] = v[0];
                data[data_index++] = v[1];
                data[data_index++] = v[2];
                data[data_index++] = 0.0f;
            } else 
                // TODO: Colocar uma exceção descente
                throw 1;
        }
    }

}