#include <GL/glew.h>
#include <string>
#include <cstdio>
#include <cstring>

#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/core/Manager.h"

using namespace pbge;

Renderer::Renderer(OpenGL * _ogl){
    this->ogl = _ogl;
    this->updater = new UpdaterVisitor;
    renderer = new BasicRendererVisitor(ogl);
}

void Renderer::setScene(const SceneManager * scene_manager) {
    scene = const_cast<SceneManager *>(scene_manager);
}

SceneManager * Renderer::getScene() {
    return scene;
}

void Renderer::updateScene(){
    scene->getSceneGraphRoot()->accept(updater);
}

void Renderer::render(){
    if(this->getScene() == NULL) return;
    pbge::SceneManager::camera_map::iterator camera;
    pbge::SceneManager::camera_map * cameras = getScene()->getCameras();
    Node * root = this->getScene()->getSceneGraphRoot();
    if (root == NULL) return;
    updateScene();
    for(camera = cameras->begin(); camera != cameras->end(); camera++) {
        renderer->setCamera(camera->second);
        root->accept(renderer);
    }
}

