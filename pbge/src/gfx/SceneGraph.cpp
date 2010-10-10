
#include <vector>
#include <string>
#include <cstdlib>

#include "math3d/math3d.h"

#include "pbge/gfx/SceneGraph.h"

namespace pbge {
    SceneManager::SceneManager() {
        ambientLight = math3d::vector4(0,0,0,0);
    }

    /*
    Node * SceneManager::getNodeByName(std::string name){
        std::map<std::string,Node*>::iterator node = nodes.find(name);
        if(node != nodes.end()) return node->second;
        return NULL;
    }
    */

    void SceneManager::addCamera(Camera * camera, const std::string & cameraName) {
        if(cameraName != "")
            camera->setName(cameraName);
        this->cameras[camera->getName()] = camera;
    }

    Camera * SceneManager::getCamera(const std::string & name) {
        std::map<std::string, Camera*>::iterator it = cameras.find(name);
        if (it != cameras.end())
            return it->second;
        return NULL;
    }

    void SceneManager::setAmbientLightColor(const math3d::vector4 & color) {
        ambientLight = color;
    }

    void SceneManager::setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha) {
        ambientLight = math3d::vector4(red, green, blue, alpha);
    }

    const math3d::vector4 SceneManager::getAmbientLightColor() const {
        return ambientLight;
    }
}
