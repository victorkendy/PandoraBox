#include <GL/glew.h>
#include <algorithm>
#include <string>
#include <functional>
#include <boost/bind/bind.hpp>

#include "pbge/core/algorithm.h"
#include "pbge/gfx/DepthBufferController.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Geometrics.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/DrawController.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/core/Manager.h"
#include "pbge/internal/OpenGLStates.h"

#include "pbge/gfx/processors/RenderPassProcessor.h"

using namespace pbge;

Renderer::Renderer(boost::shared_ptr<GraphicAPI> _ogl): updater(new UpdaterVisitor),
                        renderer(new ColorPassVisitor),
                        depthRenderer(new DepthPassVisitor),
                        lightPassVisitor(new LightPassVisitor),
                        ogl(_ogl) {
    quad.reset(Geometrics::createSquare(2.0f, ogl.get()));
    Texture2D * colorOut = ogl->getFactory()->create2DTexture();
    Texture2D * depthOut = ogl->getFactory()->create2DTexture();
    renderables["color"] = colorOut;
    renderables["depth"] = depthOut;
    fbo.reset(ogl->getFactory()->createFramebuffer(1024,768));
    fbo->addRenderable(colorOut, "color");
    fbo->setDepthRenderable(depthOut);
    sceneProcessors.push_back(new RenderPassProcessor);
}

void Renderer::initialize() {
    pbge::cond_for_each(
        postProcessors.begin(), 
        postProcessors.end(),
        boost::bind(&ScenePostProcessor::initialize, _1, ogl.get(), this),
        std::not1(std::bind2nd(std::mem_fun(&ScenePostProcessor::isInitialized), ogl.get())));
    pbge::cond_for_each(
        sceneProcessors.begin(),
        sceneProcessors.end(),
        boost::bind(&SceneProcessor::initialize, _1, ogl.get(), this),
        std::not1(std::bind2nd(std::mem_fun(&SceneProcessor::isInitialized), ogl.get())));
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

void Renderer::renderWithCamera(Node * root, Camera * camera) {
    camera->setCamera(ogl.get());
    //ogl->disableDrawBuffer();
    //depthRenderer->visit(root, ogl.get());
    //ogl->enableDrawBuffer(GL_BACK);
    //ogl->depthMask(GL_FALSE);
    /*ogl->enable(GL_BLEND);
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
    
    ogl->disable(GL_BLEND);*/

    pbge::cond_for_each(
        sceneProcessors.begin(),
        sceneProcessors.end(),
        boost::bind(&SceneProcessor::process, _1, ogl.get(), this),
        std::mem_fun(&SceneProcessor::isActive));
    camera->unsetCamera(ogl.get());
}

void Renderer::render(){
    DepthBufferController * depth = ogl->depthBufferController();
    ogl->bindFramebufferObject(fbo.get());
    ogl->updateState();
    depth->enableDepthTest();
    depth->enableDepthWrite();
    depth->useDepthFunc(DepthBufferController::DEPTH_LESS_EQUAL);

    ogl->clear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    Node * root = this->getScene()->getSceneGraphRoot();
    if(this->getScene() == NULL || root == NULL) return;
    updateScene();
    std::vector<Camera*> & activeCameras = updater->getActiveCameras();
    std::for_each(
        activeCameras.begin(),
        activeCameras.end(),
        boost::bind(&Renderer::renderWithCamera, boost::ref(this), boost::ref(root), _1));
    depth->disableDepthTest();
    depth->disableDepthWrite();
    pbge::cond_for_each(
        postProcessors.begin(),
        postProcessors.end(),
        boost::bind(&ScenePostProcessor::process, _1, ogl.get(), this),
        std::mem_fun(&ScenePostProcessor::isActive));
}

void Renderer::renderScreenQuad(GPUProgram * program) {
    ogl->getState()->useProgram(program);
    ogl->getDrawController()->drawVBOModel(quad.get());
}