#include <GL/glew.h>
#include <algorithm>
#include <string>

#include "pbge/gfx/FramebufferObject.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
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


#include "pbge/gfx/processors/BlitToFramebuffer.h"

using namespace pbge;

class PostProcessorExecutor {
public:
    inline PostProcessorExecutor(GraphicAPI * _gfx, Renderer * _renderer)
        :gfx(_gfx), renderer(_renderer) {}
    inline void operator () (ScenePostProcessor * processor) {
        if(processor->isActive()) {
            processor->process(gfx, renderer);
        }
    }
private:
    GraphicAPI * gfx;
    Renderer * renderer;
};
class PostProcessorInitializer {
public:
    inline PostProcessorInitializer(GraphicAPI * _gfx, Renderer * _renderer)
        :gfx(_gfx), renderer(_renderer) {}
    inline void operator() (ScenePostProcessor * processor) {
        if(!processor->isInitialized(gfx)) {
            processor->initialize(gfx, renderer);
        }
    }
private:
    GraphicAPI * gfx;
    Renderer * renderer;
};

Renderer::Renderer(boost::shared_ptr<GraphicAPI> _ogl): updater(new UpdaterVisitor),
                        renderer(new ColorPassVisitor),
                        depthRenderer(new DepthPassVisitor),
                        lightPassVisitor(new LightPassVisitor),
                        ogl(_ogl) {
    Texture2D * colorOut = ogl->getFactory()->create2DTexture();
    Texture2D * depthOut = ogl->getFactory()->create2DTexture();
    renderables["color"] = colorOut;
    renderables["depth"] = depthOut;
    fbo.reset(ogl->getFactory()->createFramebuffer(1024,768));
    fbo->addRenderable(colorOut, "color");
    fbo->setDepthRenderable(depthOut);
}

void Renderer::initialize() {
    std::for_each(postProcessors.begin(), postProcessors.end(), PostProcessorInitializer(ogl.get(), this));
}

Texture2D * Renderer::getColorTexture() {
    return renderables["color"];
}

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
    camera->setCamera(ogl.get());
    //ogl->disableDrawBuffer();
    //depthRenderer->visit(root, ogl.get());
    //ogl->enableDrawBuffer(GL_BACK);
    //ogl->depthMask(GL_FALSE);
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
    ogl->bindFramebufferObject(fbo.get());
    ogl->updateState();
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
    std::for_each(postProcessors.begin(), postProcessors.end(), PostProcessorExecutor(ogl.get(), this));
}

