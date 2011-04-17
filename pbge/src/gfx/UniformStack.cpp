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
        UniformValue * value = (*set)->searchValue(info);
        if(value != NULL) {
            return value;
        }
    }
    return NULL;
}

UniformSet * UniformStack::getGloabalUniforms() {
    return stack.at(0);
}

void UniformStack::push(UniformSet *uniforms) {
    stack.push_back(uniforms);
}

void UniformStack::pop() {
    stack.pop_back();
}
