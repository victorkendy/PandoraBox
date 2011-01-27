

#ifndef PBGE_GFX_SHADERUNIFORM_H_
#define PBGE_GFX_SHADERUNIFORM_H_

#include <string>
#include <sstream>

#include "pbge/gfx/OpenGL.h"


namespace pbge {

    class GPUProgram;
    class OpenGL;

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
        UniformInfo(const std::string & _name, const GLenum _type, const int & _location) {
            name = _name;
            type = translateGLType(_type);
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
            if(this->getType() < other.getType()) return true;
            return this->getName() < other.getName();
        }

        std::string toString() { 
            std::ostringstream os(std::ostringstream::out);
            os << name << " " << location << " " << type << std::endl;
            return os.str();
        }
    private:
        UniformType translateGLType(GLenum type) {
            switch(type) {
                case GL_FLOAT: return FLOAT; break;
                case GL_FLOAT_VEC2: return FLOAT_VEC2; break;
                case GL_FLOAT_VEC3: return FLOAT_VEC3; break;
                case GL_FLOAT_VEC4: return FLOAT_VEC4; break;
                case GL_INT: return INT; break;
                case GL_INT_VEC2: return INT_VEC2; break;
                case GL_INT_VEC3: return INT_VEC3; break;
                case GL_INT_VEC4: return INT_VEC4; break;
                case GL_BOOL: return INT; break;
                case GL_BOOL_VEC2: return INT_VEC2; break;
                case GL_BOOL_VEC3: return INT_VEC3; break;
                case GL_BOOL_VEC4: return INT_VEC4; break;
                case GL_FLOAT_MAT2: return FLOAT_MAT2; break;
                case GL_FLOAT_MAT3: return FLOAT_MAT3; break;
                case GL_FLOAT_MAT4: return FLOAT_MAT4; break;
                case GL_FLOAT_MAT2x3: return FLOAT_MAT23; break;
                case GL_FLOAT_MAT2x4: return FLOAT_MAT24; break;
                case GL_SAMPLER_2D: return SAMPLER_2D; break;
                case GL_SAMPLER_3D: return SAMPLER_3D; break;
                case GL_SAMPLER_CUBE: return SAMPLER_CUBE; break;
                case GL_SAMPLER_1D_SHADOW: return SAMPLER_1D_SHADOW; break;
                case GL_SAMPLER_2D_SHADOW: return SAMPLER_2D_SHADOW; break;
                default: return INVALID; break;
            }
        }

        int location;        

        UniformType type;

        std::string name;
    };

    class UniformValue {
    public:
        virtual UniformType getType() = 0;

        virtual void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl) = 0;
    };

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

        void bindValueOn(GPUProgram * program, const UniformInfo & info, OpenGL * ogl);

    private:
        GLfloat values[4];
    };

    
}
#endif
