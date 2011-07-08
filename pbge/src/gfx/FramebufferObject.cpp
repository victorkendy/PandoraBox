#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <boost/function.hpp>

#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/FramebufferObject.h"

using namespace pbge;

void FramebufferObject::bind() {
    if(!isInitialized()) {
        initialize();
    }
    if(!bound) {
        bindFramebuffer();
        bound = true;
    }
    std::vector<Texture2D*> diff(std::max(added.size(), unsync_added.size()));
    std::vector<Texture2D*>::iterator end;
    // Add the new renderables
    end = std::set_difference(unsync_added.begin(), unsync_added.end(), added.begin(), added.end(), diff.begin());
    std::for_each(diff.begin(), end, std::bind1st(std::mem_fun(&FramebufferObject::validateAndAttachRenderable), this));
    // Dettach the removed renderables
    end = std::set_difference(added.begin(), added.end(), unsync_added.begin(), unsync_added.end(), diff.begin());
    std::for_each(diff.begin(), end, std::bind1st(std::mem_fun(&FramebufferObject::dettachRenderable), this));
    // Synchronize the sets
    added = unsync_added;
}

void FramebufferObject::validateAndAttachRenderable(Texture2D * texture) {
	if(texture->getWidth() == width && texture->getHeight() == height) {
		attachRenderable(texture);
	} else {
		throw pbge::IllegalArgumentException("The renderable size is not valid for this FBO");
	}
}

void FramebufferObject::unbind() {
    if(bound) {
        unbindFramebuffer();
        bound = false;
    }
}

