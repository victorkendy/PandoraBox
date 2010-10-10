
#ifndef gfxsnode_h_
#define gfxsnode_h_

#include <vector>
#include <string>
#include <map>

#include "math3d/math3d.h"

#include "pbge/core/Object.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/Node.h"

namespace pbge {

    class PBGE_EXPORT SceneManager: public Object{
    public:
        typedef std::map<std::string, Camera *> camera_map;
        typedef std::map<std::string, Node *> node_map;

        SceneManager();

        // Node * getNodeByName(std::string name);

        void addCamera(Camera * camera, const std::string & cameraName="");

        /*
        void removeCamera(const Camera * camera) {
            cameras.erase(camera->getName());
        }

        void removeCamera(const std::string & cameraName) {
            cameras.erase(cameraName);
        }
        */

        Camera * getCamera(const std::string & name);
        
        camera_map * getCameras() {
            return &cameras; 
        }

        void setSceneGraph(const Node * root) {
            scene_graph = const_cast<Node *>(root);
        }

        Node * getSceneGraphRoot() {
            return scene_graph;
        }

        void setAmbientLightColor(const math3d::vector4 & color);
        void setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha);

        const math3d::vector4 getAmbientLightColor() const;
    private:
        math3d::vector4 ambientLight;
        Node * scene_graph;
        camera_map cameras;
        node_map nodes;
    };
}
#endif

