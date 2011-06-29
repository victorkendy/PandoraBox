

#ifndef PBGE_GFX_SHADERUNIFORM_H_
#define PBGE_GFX_SHADERUNIFORM_H_
#include <cstring>
#include <string>
#include <sstream>

#include "math3d/math3d.h"

#include "pbge/core/PairCompare.h"

namespace pbge {

    class GPUProgram;
    class GraphicAPI;
    class Texture2D;
    class Texture1D;

    typedef enum {
        INVALID, 

        FLOAT,
        FLOAT_VEC2,
        FLOAT_VEC3,
        FLOAT_VEC4,

        INT,
        INT_VEC2,
        INT_VEC3,
        INT_VEC4,

        BOOL,
        BOOL_VEC2,
        BOOL_VEC3,
        BOOL_VEC4,

        FLOAT_MAT2,
        FLOAT_MAT3,
        FLOAT_MAT4,
        FLOAT_MAT23,
        FLOAT_MAT24,
        
        SAMPLER_1D,
        SAMPLER_2D,
        SAMPLER_3D,
        SAMPLER_CUBE,
        SAMPLER_1D_SHADOW,
        SAMPLER_2D_SHADOW
    }UniformType;

    class UniformInfo {
    public:
        UniformInfo(const std::string & _name, const UniformType _type, const int & _location = -1, const unsigned _numberOfElements = 1) {
            name = _name;
            type = _type;
            location = _location;
            numberOfElements = _numberOfElements;
        }

        UniformInfo(){}

        UniformType getType() const{
            return type;
        }

        const std::string getName() const{
            return name;
        }

        const int getLocation() const{
            return location;
        }

        const unsigned getNumberOfElements() const {
            return numberOfElements;
        }
        
        bool operator < (const UniformInfo other) const {
            return PairCompare<UniformType, std::string>::less(getType(), other.getType(), getName(), other.getName());
        }

        std::string toString() { 
            std::ostringstream os(std::ostringstream::out);
            os << name << " " << location << " " << type << " " << numberOfElements << std::endl;
            return os.str();
        }

    private:

        int location;        

        UniformType type;

        unsigned numberOfElements;

        std::string name;
    };

    class UniformValue {
    public:
        UniformValue() {
            numberOfElements = 0;
            valueStorage = NULL;
        }

        ~UniformValue() {
            if(valueStorage != NULL) {
                free(valueStorage);
            }
            numberOfElements = 0;
        }

        virtual UniformType getType() = 0;

        virtual unsigned getTypeSize() = 0;

        virtual void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl) = 0;

    protected:
        void * getValueAt(unsigned index) {
            if(index >= numberOfElements) {
                resize(index);
            }
            return ((unsigned char *)(valueStorage)) + (index * getTypeSize());
        }

        const unsigned getNumberOfElements() const {
            return numberOfElements;
        }
    private:
        void resize(unsigned index) {
            unsigned _numberOfElements = numberOfElements + 1;
            unsigned typeSize = getTypeSize();
            float reallocFactor = 2.0f;
            while(_numberOfElements <= index) {
                _numberOfElements = static_cast<unsigned>(_numberOfElements * reallocFactor);
            }
            void * _valueStorage = malloc(typeSize * _numberOfElements);
            if(numberOfElements > 0) {
                memcpy(_valueStorage, valueStorage, numberOfElements * typeSize);
            }
            numberOfElements = _numberOfElements;
            free(valueStorage);
            valueStorage = _valueStorage;
        }

        void * valueStorage;

        unsigned numberOfElements;
    };

    // Declaration of the UniformValues

    class UniformFloatVec4 : public UniformValue {
    public:
        UniformFloatVec4() : UniformValue() {
            setValueAt(0, 0.0f, 0.0f, 0.0f, 0.0f);
        }

        UniformFloatVec4(const float & x, const float & y, const float & z, const float & w) : UniformValue() {
            setValueAt(0, x,y,z,w);
        }

        UniformType getType() {
            return FLOAT_VEC4;
        }

        unsigned getTypeSize() {
            return 4 * sizeof(float);
        }

        void setValueAt(unsigned index, const float & x, const float & y, const float & z, const float & w) {
            float * valueAtIndex = static_cast<float *>(this->getValueAt(index));
            valueAtIndex[0] = x;
            valueAtIndex[1] = y;
            valueAtIndex[2] = z;
            valueAtIndex[3] = w;
        }
        
        void setValue(const float & x, const float & y, const float & z, const float & w) {
            setValueAt(0, x,y,z,w);
        }

        void setValue(const math3d::vector4 & v) {
            setValueAt(0, v[0], v[1], v[2], v[3]);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    };

    class UniformFloatVec3 : public UniformValue {
    public:
        UniformFloatVec3() : UniformValue() {
            setValueAt(0, 0.0f, 0.0f, 0.0f);
        }

        UniformFloatVec3(const float & x, const float & y, const float & z) : UniformValue(){
            setValueAt(0, x, y, z);
        }

        UniformType getType() {
            return FLOAT_VEC3;
        }

        unsigned getTypeSize() {
            return 3 * sizeof(float);
        }

        void setValueAt(unsigned index, const float & x, const float & y, const float & z) {
            float * values = static_cast<float *>(getValueAt(index));
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }
        
        void setValue(const float & x, const float & y, const float & z) {
            setValueAt(0, x, y, z);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    };

    class UniformFloatVec2 : public UniformValue {
    public:
        UniformFloatVec2() : UniformValue(){
            setValueAt(0, 0.0f, 0.0f);
        }

        UniformFloatVec2(const float & x, const float & y) : UniformValue(){
            setValueAt(0, x, y);
        }

        UniformType getType() {
            return FLOAT_VEC2;
        }

        unsigned getTypeSize() {
            return 2 * sizeof(float);
        }
        
        void setValueAt(unsigned index, const float & x, const float & y) {
            float * values = static_cast<float *>(getValueAt(index));
            values[0] = x;
            values[1] = y;
        }

        void setValue(const float & x, const float & y) {
            setValueAt(0, x, y);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    };

    class UniformFloat : public UniformValue {
    public:
        UniformFloat() : UniformValue(){
            setValueAt(0, 0.0f);
        }

        UniformFloat(const float & v) :UniformValue(){
            setValueAt(0, v);
        }

        UniformType getType() {
            return FLOAT;
        }

        unsigned getTypeSize() {
            return sizeof(float);
        }

        void setValueAt(unsigned index, const float & x) {
            float * value = static_cast<float *>(getValueAt(0));
            *value = x;
        }
        
        void setValue(const float & v) {
            setValueAt(0, v);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    };

    class UniformSampler1D : public UniformValue {
    public:
        UniformSampler1D() : UniformValue() {
            this->texture = NULL;
        }
        void setValue(Texture1D * tex) {
            this->texture = tex;
        }
        UniformType getType() { return SAMPLER_1D; }

        unsigned getTypeSize() {
            return sizeof(Texture1D*);
        }
        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    private:
        Texture1D * texture;
    };

    class UniformSampler2D : public UniformValue {
    public:
        UniformSampler2D() : UniformValue() {
            this->texture = NULL;
        }

        void setValue(Texture2D * value) {
            this->texture = value;
        }
        
        UniformType getType() { return SAMPLER_2D; }

        unsigned getTypeSize() {
            return sizeof(Texture2D *);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    private:
        Texture2D * texture;
    };

    class UniformMat4 : public UniformValue {
    public:
        UniformMat4() : UniformValue() {
            values[0] = 0.0f;
            values[1] = 0.0f;
            values[2] = 0.0f;
            values[3] = 0.0f;
            values[4] = 0.0f;
            values[5] = 0.0f;
            values[6] = 0.0f;
            values[7] = 0.0f;
            values[8] = 0.0f;
            values[9] = 0.0f;
            values[10] = 0.0f;
            values[11] = 0.0f;
            values[12] = 0.0f;
            values[13] = 0.0f;
            values[14] = 0.0f;
            values[15] = 0.0f;
        }

        void setValue(float a00, float a01, float a02, float a03,
                      float a10, float a11, float a12, float a13,
                      float a20, float a21, float a22, float a23,
                      float a30, float a31, float a32, float a33) {
            values[0] = a00;
            values[1] = a10;
            values[2] = a20;
            values[3] = a30;
            values[4] = a01;
            values[5] = a11;
            values[6] = a21;
            values[7] = a31;
            values[8] = a02;
            values[9] = a12;
            values[10] = a22;
            values[11] = a32;
            values[12] = a03;
            values[13] = a13;
            values[14] = a23;
            values[15] = a33;
        }

        void setValue(const math3d::matrix44 & matrix) {
            values[0] = matrix[0][0];
            values[1] = matrix[1][0];
            values[2] = matrix[2][0];
            values[3] = matrix[3][0];
            values[4] = matrix[0][1];
            values[5] = matrix[1][1];
            values[6] = matrix[2][1];
            values[7] = matrix[3][1];
            values[8] = matrix[0][2];
            values[9] = matrix[1][2];
            values[10] = matrix[2][2];
            values[11] = matrix[3][2];
            values[12] = matrix[0][3];
            values[13] = matrix[1][3];
            values[14] = matrix[2][3];
            values[15] = matrix[3][3];
        }

        UniformType getType() { return FLOAT_MAT4; }

        unsigned getTypeSize() {
            return 16 * sizeof(float);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, GraphicAPI * ogl);
    private:
        float values[16];
    };
}
#endif
