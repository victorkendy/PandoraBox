

#ifndef PBGE_GFX_VBO_H_
#define PBGE_GFX_VBO_H_

#include <cstddef>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/exceptions/exceptions.h"

namespace pbge {
    class VertexBuffer;
    class GraphicAPI;
    class Buffer;

    /** This class represents a vertex attribute of a vertex buffer

        @author Victor Kendy Harada
    */
    class PBGE_EXPORT VertexAttrib{
    public:
        typedef enum {
            VERTEX,
            NORMAL,
            TEXCOORD,
            COLOR,
            SECONDARY_COLOR,
            CUSTOM_ATTRIB
        } Type;

        VertexAttrib(int _nCoord, int _offset, size_t _stride, Type _type, const std::string & _name)
                        :nCoord(_nCoord),
                         offset(_offset),
                         stride(_stride),
                         type(_type),
                         name(_name) {}

        int getOffset() {
            return offset;
        }
        int getStride() {
            return stride;
        }
        int getNCoord() {
            return nCoord;
        }
        Type getType() {
            return type;
        }
        std::string getName() {
            return name;
        }
    private:
        Type type;
        std::string name;
        int nCoord, offset;
        size_t stride;
    };

    class PBGE_EXPORT VertexBuffer {
    public:
        typedef std::vector<VertexAttrib>::iterator AttrIter;

        VertexBuffer(Buffer * _buffer, unsigned _nVertices):buffer(_buffer), nVertices(_nVertices){
        }

        ~VertexBuffer() {
        }

        void addAttrib(VertexAttrib attrib) {
            attribs.push_back(attrib);
        }

        std::vector<VertexAttrib> & getAttribs() {
            return attribs;
        }

        VertexAttrib * findByType(VertexAttrib::Type type) {
            for(AttrIter it = attribs.begin(); it != attribs.end(); it++) {
                if(it->getType() == type)
                    return &(*it);
            }
            return NULL;
        }

        VertexAttrib * findByName(const std::string & name) {
            for(AttrIter it = attribs.begin(); it != attribs.end(); it++) {
                if(it->getName() == name) 
                    return &(*it);
            }
            return NULL;
        }

        Buffer * getBuffer() {
            return buffer.get();
        }

        unsigned getNVertices() {
            return nVertices;
        }
    
    private:
        boost::scoped_ptr<Buffer> buffer;
        unsigned nVertices;
        std::vector<VertexAttrib> attribs;
    };



    class PBGE_EXPORT VertexAttribBuilder {
    public:
        VertexAttribBuilder(unsigned _nCoord, VertexAttrib::Type _type, int _index = -1):nCoord(_nCoord),
                             type(_type),
                             index(_index),
                             indexesAssigned(false),
                             currentElement(0),name("") {}

        VertexAttribBuilder(unsigned _nCoord, VertexAttrib::Type _type, const std::string & _name):nCoord(_nCoord),
                               type(_type),
                               index(-1),
                               indexesAssigned(false),
                               currentElement(0),
                               name(_name) {}

        void pushValue(const float x, const float y, const float z, const float w) {
            values.push_back(x);
            values.push_back(y);
            values.push_back(z);
            values.push_back(w);
        }

        bool operator == (const VertexAttribBuilder & other);

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

        const VertexAttrib createInstance(int offset, size_t stride);

    private:
        std::string name;
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
        VertexBufferBuilder(unsigned _nVertices):nVertices(_nVertices),curAttrib(NULL) {}

        const VertexAttribBuilder addAttrib(unsigned _nElements, VertexAttrib::Type _type, int _index=-1) {
            VertexAttribBuilder attrib(_nElements, _type, _index);
            attribs.push_back(attrib);
            return attrib;
        }

        VertexBufferBuilder & pushValue(const VertexAttribBuilder & attrib, const float &x=0.0f, const float & y=0.0f, const float & z=0.0f, const float & w=0.0f);
        
        VertexBufferBuilder & pushValue(const float &x=0.0f, const float & y=0.0f, const float & z=0.0f, const float & w=0.0f) {
            if(curAttrib != NULL) curAttrib->pushValue(x,y,z,w);
            return *this;
        }

        VertexBufferBuilder & on(const VertexAttribBuilder & attrib) {
            std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
            if(it != attribs.end())
                curAttrib = &(*it);
            return *this;
        }

        VertexBufferBuilder & setAttribIndex(const VertexAttribBuilder & attrib, const std::vector<unsigned short> & indexes);

        VertexBufferBuilder & setAttribIndex(const std::vector<unsigned short> & indexes);

        VertexBuffer * done(Buffer::UsageHint usage, GraphicAPI * gfx);

    private:
        size_t calculateSize();

        VertexAttribBuilder * curAttrib;

        void createAttribs(VertexBuffer * vbo, size_t stride);

        unsigned nVertices;

        std::vector<VertexAttribBuilder> attribs;
    };
}

#endif