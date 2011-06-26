#include <algorithm>
#include <map>
#include <string>
#include <boost/function.hpp>

#include "pbge/gfx/FramebufferObject.h"

using namespace pbge;

template<typename K, typename V>
class MapValue {
public:
    MapValue(boost::function1<void,V> op):operation(op) {}
    void operator()(const std::pair<K,V> & p) {
        operation(p.second);
    }
private:
    boost::function1<void,V> operation;
};

void FramebufferObject::bind() {
    if(!isInitialized()) {
        initialize();
    }
    std::for_each(renderables.begin(), renderables.end(), MapValue<std::string,Texture2D*>(std::bind1st(std::mem_fun(&FramebufferObject::attachRenderable), this)));
    bindFramebuffer();
}

void FramebufferObject::unbind() {
    unbindFramebuffer();
}