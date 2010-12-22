

#ifndef gfxvbo_h_
#define gfxvbo_h_

#include <vector>
#include <algorithm>

#include "pbge/core/core.h"
#include "pbge/core/Vec3.h"

namespace pbge {
    class PBGE_EXPORT VertexAttrib {
    public:
        typedef enum {
            VERTEX,
            NORMAL,
            TEXCOORD,
            COLOR,
            SECONDARY_COLOR,
            CUSTOM_ATTRIB
        } Type;
    };

    class PBGE_EXPORT VertexBuffer {
    public:
        
    };

    class PBGE_EXPORT VertexAttribBuilder {
    public:
        VertexAttribBuilder(unsigned _nCoord, VertexAttrib::Type _type, int _index) {
            nCoord = _nCoord;
            type = _type;
            index = _index;
            indexesAssigned = false;
            currentElement = 0;
        }

        VertexAttribBuilder(unsigned _nCoord, VertexAttrib::Type _type) {
            nCoord = _nCoord;
            type = _type;
            index = -1;
            indexesAssigned = false;
            currentElement = 0;
        }

        void pushValue(const float x, const float y, const float z, const float w) {
            values.push_back(x);
            values.push_back(y);
            values.push_back(z);
            values.push_back(w);
        }

        bool operator == (const VertexAttribBuilder & other) {
            return (this->type == other.type && this->index == other.index && this->nCoord == other.nCoord);
        }

        void setIndexes(const std::vector<unsigned short> & _indexes) {
            indexesAssigned = true;
            this->indexes = _indexes;
        }

        int numberOfCoordinates() {
            return nCoord;
        }

        int numberOfElements() {
            return indexes.size();
        }

        bool areIndexesAssigned() {
            return this->indexesAssigned;
        }

        void getNextElement(float * elems);

        bool isValid();

    private:
        unsigned nCoord;
        VertexAttrib::Type type;
        int index;
        std::vector<float> values;
        bool indexesAssigned;
        std::vector<unsigned short> indexes;
        int currentElement;
    };


    class PBGE_EXPORT VertexBufferBuilder {
    public:
        VertexBufferBuilder(unsigned _nVertices) {
            nVertices = _nVertices;
        }

        const VertexAttribBuilder addAttrib(unsigned _nElements, VertexAttrib::Type _type, int _index=-1) {
            VertexAttribBuilder attrib(_nElements, _type, _index);
            attribs.push_back(attrib);
            return attrib;
        }

        VertexBufferBuilder & pushValue(const VertexAttribBuilder & attrib, const float &x=0.0f, const float & y=0.0f, const float & z=0.0f, const float & w=0.0f) {
            std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
            if(it == attribs.end()) {
                VertexAttribBuilder newAttrib(attrib);
                newAttrib.pushValue(x,y,z,w);
                attribs.push_back(newAttrib);
            } else {
                it->pushValue(x,y,z,w);
            }
            return *this;
        }

        VertexBufferBuilder & setAttribIndex(const VertexAttribBuilder & attrib, const std::vector<unsigned short> & indexes) {
            std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
            if(it == attribs.end()) {
                VertexAttribBuilder newAttrib(attrib);
                newAttrib.setIndexes(indexes);
                attribs.push_back(newAttrib);
            } else {
                it->setIndexes(indexes);
            }
            return *this;
        }

        VertexBuffer * done(GLenum usage = GL_STATIC_DRAW);

    private:
        size_t calculateSize();
        unsigned nVertices;
        std::vector<VertexAttribBuilder> attribs;
    };
}

#endif