

#ifndef PBGE_GFX_SHADERUNIFORM_H_
#define PBGE_GFX_SHADERUNIFORM_H_


#include <string>
#include <sstream>

#include "math3d/math3d.h"

namespace pbge {

    class GPUProgram;
    class OpenGL;
    class Texture;

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
        
        SAMPLER_2D,
        SAMPLER_3D,
        SAMPLER_CUBE,
        SAMPLER_1D_SHADOW,
        SAMPLER_2D_SHADOW
    }UniformType;

    class UniformInfo {
    public:
        UniformInfo(const std::string & _name, const UniformType _type, const int & _location) {
            name = _name;
            type = _type;
            location = _location;
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
        
        bool operator < (const UniformInfo other) const {
            return (this->getType() < other.getType() || (this->getType() == other.getType() && this->getName() < other.getName()));
        }

        std::string toString() { 
            std::ostringstream os(std::ostringstream::out);
            os << name << " " << location << " " << type << std::endl;
            return os.str();
        }

    private:

        int location;        

        UniformType type;

        std::string name;
    };

    class UniformValue {
    public:
        virtual UniformType getType() = 0;

        virtual void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl) = 0;
    };

    // Declaration of the UniformValues

    class UniformFloatVec4 : public UniformValue {
    public:
        UniformFloatVec4() {
            values[0] = 0.0f;
            values[1] = 0.0f;
            values[2] = 0.0f;
            values[3] = 0.0f;
        }

        UniformFloatVec4(const float & x, const float & y, const float & z, const float & w) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
        }

        UniformType getType() {
            return FLOAT_VEC4;
        }
        
        void setValue(const float & x, const float & y, const float & z, const float & w) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
        }

        void setValue(const math3d::vector4 & v) {
            this->setValue(v[0], v[1], v[2], v[3]);
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);

    private:
        float values[4];
    };

    class UniformFloatVec3 : public UniformValue {
    public:
        UniformFloatVec3() {
            values[0] = 0.0f;
            values[1] = 0.0f;
            values[2] = 0.0f;
        }

        UniformFloatVec3(const float & x, const float & y, const float & z) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }

        UniformType getType() {
            return FLOAT_VEC3;
        }
        
        void setValue(const float & x, const float & y, const float & z) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);

    private:
        float values[3];
    };

    class UniformFloatVec2 : public UniformValue {
    public:
        UniformFloatVec2() {
            values[0] = 0.0f;
            values[1] = 0.0f;
        }

        UniformFloatVec2(const float & x, const float & y) {
            values[0] = x;
            values[1] = y;
        }

        UniformType getType() {
            return FLOAT_VEC2;
        }
        
        void setValue(const float & x, const float & y) {
            values[0] = x;
            values[1] = y;
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);

    private:
        float values[2];
    };

    class UniformFloat : public UniformValue {
    public:
        UniformFloat() {
            value = 0.0f;
        }

        UniformFloat(const float & v) {
            value = v;
        }

        UniformType getType() {
            return FLOAT;
        }
        
        void setValue(const float & v) {
            value = v;
        }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);

    private:
        float value;
    };

    class UniformSampler2D : public UniformValue {
    public:
        UniformSampler2D() {
            this->texture = NULL;
        }

        void setValue(Texture * value) {
            this->texture = value;
        }
        
        UniformType getType() { return SAMPLER_2D; }

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);
    private:
        Texture * texture;
    };

    class UniformMat4 : public UniformValue {
    public:
        UniformMat4() {
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

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);
    private:
        float values[16];
    };
}
#endif
