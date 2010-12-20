

#ifndef gfxvbo_h_
#define gfxvbo_h_

#include <vector>
#include <algorithm>

#include "pbge/core/core.h"
#include "pbge/core/Vec3.h"

namespace pbge {
    
    class PBGE_EXPORT VertexBuffer {
    public:
        class VertexAttrib {
        public:
            typedef enum {
                VERTEX,
                NORMAL,
                TERXCOORD,
                COLOR,
                SECONDARY_COLOR,
                CUSTOM_ATTRIB
            } Type;
        };
    };

    class PBGE_EXPORT VertexBufferBuilder {
    public:
        class VertexAttribBuilder {
        public:
            VertexAttribBuilder(unsigned _nElements, VertexBuffer::VertexAttrib::Type _type, int _index) {
                nElements = _nElements;
                type = _type;
                index = _index;
            }

            VertexAttribBuilder(unsigned _nElements, VertexBuffer::VertexAttrib::Type _type) {
                nElements = _nElements;
                type = _type;
                index = -1;
            }

            void pushValue(const float x, const float y=0.0f, const float z=0.0f, const float w=1.0f) {
                values.push_back(x);
                values.push_back(y);
                values.push_back(z);
                values.push_back(w);
            }

        private:
            unsigned nElements;
            VertexBuffer::VertexAttrib::Type type;
            int index;
            std::vector<float> values;
        };


        VertexBufferBuilder(unsigned _nVertices) {
            nVertices = _nVertices;
        }

        VertexAttribBuilder * addAttrib(unsigned _nElements, VertexBuffer::VertexAttrib::Type _type, int _index) {
            VertexAttribBuilder * attrib = new VertexAttribBuilder(_nElements, _type, _index);
            attribs.push_back(attrib);
            return attrib;
        }

        VertexBufferBuilder * pushValue(VertexAttribBuilder * attrib, const float &x, const float & y, const float & z, const float & w) {
            std::vector<VertexAttribBuilder*>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
            if(it == attribs.end())
                attribs.push_back(attrib);
            attrib->pushValue(x,y,z,w);
            // allows chaining
            return this;
        }

        VertexBuffer * done();

        ~VertexBufferBuilder() {
            std::vector<VertexAttribBuilder*>::iterator it;
            for(it = attribs.begin(); it != attribs.end(); it++) {
                delete *it;
            }
            attribs.clear();
        }

    private:
        size_t calculateSize();
        unsigned nVertices;
        std::vector<VertexAttribBuilder*> attribs;
    };
}

#endif