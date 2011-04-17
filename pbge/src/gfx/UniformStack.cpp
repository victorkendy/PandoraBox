#include <vector>
#include <string>

#include "pbge/gfx/UniformStack.h"
#include "pbge/gfx/UniformSet.h"

using namespace pbge;

UniformStack::UniformStack() {
    this->stack.push_back(new UniformSet());
}

// delete the global uniform set
UniformStack::~UniformStack() {
    delete this->stack.at(0);
}

UniformValue * UniformStack::findUniform(const UniformInfo &info) {
    std::vector<UniformSet *>::reverse_iterator set;
    for(set = stack.rbegin(); set != stack.rend(); set++) {
        UniformValue * value = (*set)->getValue(info);
        if(value != NULL) {
            return value;
        }
    }
    return NULL;
}

UniformSet * UniformStack::getGloabalUniforms() {
    return stack.at(0);
}

UniformFloat * UniformStack::findFloat(const std::string & name) {
    return dynamic_cast<UniformFloat*>(findUniform(UniformInfo(name, pbge::FLOAT, -1)));
}

UniformFloatVec2 * UniformStack::findFloatVec2(const std::string & name) {
    return dynamic_cast<UniformFloatVec2*>(findUniform(UniformInfo(name, pbge::FLOAT_VEC2, -1)));
}

UniformFloatVec3 * UniformStack::findFloatVec3(const std::string & name) {
    return dynamic_cast<UniformFloatVec3*>(findUniform(UniformInfo(name, pbge::FLOAT_VEC3, -1)));
}

UniformFloatVec4 * UniformStack::findFloatVec4(const std::string & name) {
    return dynamic_cast<UniformFloatVec4*>(findUniform(UniformInfo(name, pbge::FLOAT_VEC4, -1)));
}