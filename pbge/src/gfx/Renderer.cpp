#include <boost/smart_ptr/shared_ptr.hpp>
#include <GL/glew.h>
#include <string>
#include <cstring>

#include "pbge/gfx/FramebufferObject.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/core/Manager.h"
#include "pbge/internal/OpenGLStates.h"

using namespace pbge;

Renderer::Renderer(boost::shared_ptr<GraphicAPI> _ogl): updater(new UpdaterVisitor),
                        renderer(new ColorPassVisitor),
                        depthRenderer(new DepthPassVisitor),
                        lightPassVisitor(new LightPassVisitor),
                        ogl(_ogl) {}


void Renderer::setScene(boost::shared_ptr<SceneGraph> & scene_graph) {
    scene = scene_graph;
}

SceneGraph * Renderer::getScene() {
    return scene.get();
}

void Renderer::updateScene(){
    updater->visit(scene->getSceneGraphRoot(), ogl.get());
}

void Renderer::renderWithCamera(Camera * camera, Node * root) {
	/*if(fbo.get() == NULL) { 
		Texture2D * tex = ogl->getFactory()->create2DTexture();
		fbo.reset(ogl->getFactory()->createFramebuffer(500,500));
		fbo->addRenderable(tex, "tex");
		fbo->bind();
	}*/
    camera->setCamera(ogl.get());
    
    ogl->disableDrawBuffer();
    depthRenderer->visit(root, ogl.get());
    ogl->enableDrawBuffer(GL_BACK);
    ogl->depthMask(GL_FALSE);

    
    
    ogl->enable(GL_BLEND);
    ogl->blendFunc(GL_ONE, GL_ONE);
    std::vector<Light*>::iterator it;
    lightPassVisitor->setCurrentCamera(camera);
    for(it = updater->getActiveLights().begin(); it != updater->getActiveLights().end(); it++) {
        lightPassVisitor->setCurrentLight(*it);
        lightPassVisitor->visit(root, ogl.get());
    }
    ogl->blendFunc(GL_DST_COLOR, GL_ZERO);
    
    ogl->getState()->useProgram(NULL);
    renderer->visit(root, ogl.get());
    
    ogl->disable(GL_BLEND);
    camera->unsetCamera(ogl.get());
}

void Renderer::render(){
    ogl->depthMask(GL_TRUE);
    ogl->depthFunc(GL_LEQUAL);
    
    
    ogl->clear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    Node * root = this->getScene()->getSceneGraphRoot();
    if(this->getScene() == NULL || root == NULL) return;
    updateScene();
    
    std::vector<Camera*> & activeCameras = updater->getActiveCameras();
    std::vector<Camera*>::iterator camera;
    for(camera = activeCameras.begin(); camera != activeCameras.end(); camera++) {
        renderWithCamera(*camera, root);
    }
}

