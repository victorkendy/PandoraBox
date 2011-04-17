
#include "pbge/gfx/UniformSet.h"
#include <map>

using namespace pbge;

UniformSet::~UniformSet(){
    std::map<UniformInfo, UniformValue*>::iterator it;
    for(it = uniforms.begin(); it != uniforms.end(); it++) {
        delete it->second;
    }
}

UniformValue * UniformSet::createUniform(const UniformInfo &info) {
    UniformType type = info.getType();
    UniformValue * newValue = NULL;
    if(type == FLOAT_VEC4) {
        newValue = new UniformFloatVec4;
    }
    else if(type == FLOAT_VEC3) {
        newValue = new UniformFloatVec3;
    }
    else if(type == FLOAT_VEC2) {
        newValue = new UniformFloatVec2;
    }
    else if(type == FLOAT) {
        newValue = new UniformFloat;
    }
    else if(type == SAMPLER_2D) {
        newValue = new UniformSampler2D;
    }
    else if(type == FLOAT_MAT4) {
        newValue = new UniformMat4;
    }
    if(newValue != NULL) {
        uniforms[info] = newValue;
        return newValue;
    } else {
        return NULL;
    }
}

UniformValue * UniformSet::getValue(const std::string name, UniformType type) {
    return this->getValue(UniformInfo(name, type, -1));
}

UniformValue * UniformSet::getValue(const UniformInfo & info) {
    UniformValue * value = searchValue(info);
    if(value == NULL) {
        return createUniform(info);
    }
    return value;
}

UniformValue * UniformSet::searchValue(const UniformInfo & info) {
    std::map<UniformInfo, UniformValue*>::iterator value = this->uniforms.find(info);
    if(value == this->uniforms.end()) {
        return NULL;
    }
    return value->second;
}

UniformFloat * UniformSet::getFloat(const std::string & name) {
    return dynamic_cast<UniformFloat *>(this->getValue(UniformInfo(name, pbge::FLOAT, -1)));
}

UniformFloatVec2 * UniformSet::getFloatVec2(const std::string & name) {
    return dynamic_cast<UniformFloatVec2 *>(this->getValue(UniformInfo(name, pbge::FLOAT_VEC2, -1)));
}

UniformFloatVec3 * UniformSet::getFloatVec3(const std::string & name) {
    return dynamic_cast<UniformFloatVec3 *>(this->getValue(UniformInfo(name, pbge::FLOAT_VEC3, -1)));
}

UniformFloatVec4 * UniformSet::getFloatVec4(const std::string & name) {
    return dynamic_cast<UniformFloatVec4 *>(this->getValue(UniformInfo(name, pbge::FLOAT_VEC4, -1)));
}