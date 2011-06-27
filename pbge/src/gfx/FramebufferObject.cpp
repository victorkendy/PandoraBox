#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <boost/function.hpp>

#include "pbge/gfx/FramebufferObject.h"

using namespace pbge;

void FramebufferObject::bind() {
    if(!isInitialized()) {
        initialize();
    }
    bindFramebuffer();
    std::vector<Texture2D*> diff(std::max(added.size(), unsync_added.size()));
    std::vector<Texture2D*>::iterator end;
    // Add the new renderables
    end = std::set_difference(unsync_added.begin(), unsync_added.end(), added.begin(), added.end(), diff.begin());
    std::for_each(diff.begin(), end, std::bind1st(std::mem_fun(&FramebufferObject::attachRenderable), this));
    // Dettach the removed renderables
    end = std::set_difference(added.begin(), added.end(), unsync_added.begin(), unsync_added.end(), diff.begin());
    std::for_each(diff.begin(), end, std::bind1st(std::mem_fun(&FramebufferObject::dettachRenderable), this));
    // Synchronize the sets
    added = unsync_added;
}

void FramebufferObject::unbind() {
    unbindFramebuffer();
}

