#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <boost/function.hpp>

#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/FramebufferObject.h"

using namespace pbge;

void FramebufferObject::bind(GraphicAPI * api) {
    if(!isInitialized()) {
        initialize();
    }
    if(!bound) {
        bindFramebuffer();
        bound = true;
    }
    synchronize();
    bindRenderablesToOutput(api);
}

void FramebufferObject::synchronize() {
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
	if(boundDepth != depth) {
		boundDepth = depth;
		attachDepthRenderable(depth);
	}
}

void FramebufferObject::validateAndAttachRenderable(Texture2D * texture) {
	if(texture->getWidth() == 0 && texture->getHeight() == 0) {
		texture->setImageData(pbge::Texture::UNSIGNED_BYTE, pbge::Texture::RGBA, NULL, 0, width, height, pbge::Texture::RGBA);
	}
	if(texture->getWidth() == width && texture->getHeight() == height) {
		if (!texture->isInitialized()) { 
			texture->initialize();
		}
		texture->bindTextureOn(NULL);
		attachRenderable(texture);
	} else {
		throw pbge::IllegalArgumentException("The renderable size is not valid for this FBO");
	}
}

void FramebufferObject::bindRenderablesToOutput (GraphicAPI * api) {
    GPUProgram* program = api->getCurrentProgram();
    // TODO: remove the hard coded 16
    std::vector<Texture2D*> textures(16);
    if(program == NULL) return;
    int boundNames = 0;
    std::map<std::string,Texture2D*>::iterator it;
    for(it = renderables.begin(); it != renderables.end(); it++) {
        int location = program->getOutputLocation(it->first);
        if(location != -1) {
            textures[location] = it->second;
            boundNames++;
        }
    }
    textures.resize(boundNames);
    useRenderables(textures);
}

void FramebufferObject::unbind() {
    if(bound) {
        unbindFramebuffer();
        bound = false;
    }
}

void FramebufferObject::setDepthRenderable(Texture2D * tex) {
	if(tex->getWidth() == width && tex->getHeight() == height) {
		depth = tex;
		if(isBound()) {
			boundDepth = depth;
			attachDepthRenderable(depth);
		}
	} else {
		throw pbge::IllegalArgumentException("The depth renderable size is not valid for this FBO");
	}
}