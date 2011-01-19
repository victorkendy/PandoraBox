
#include <vector>
#include <string>
#include <cstdlib>

#include "math3d/math3d.h"

#include "pbge/gfx/Node.h"
#include "pbge/gfx/SceneGraph.h"

namespace pbge {

    SceneGraph::~SceneGraph() {
        std::vector<Node*>::iterator it;
        for(it = nodes.begin(); it != nodes.end(); it++)
            delete *it;
    }

    Node * SceneGraph::appendChildTo(int index, Node * child) {
        Node * parent = nodes.at(index);
        parent->addChild(child);
        if(child->getSceneGraphIndex() < 0) {
            nodes.push_back(child);
            child->setSceneGraphIndex(nodes.size() - 1);
        }
        return child;
    }



    Node * SceneGraph::appendChildTo(Node * parent, Node * child) {
        return appendChildTo(parent->getSceneGraphIndex(), child);
    }

    Node * SceneGraph::getGraphNode(int name) {
        if (static_cast<int>(nodes.size()) > name && name >= 0)
            return nodes.at(name);
        return NULL;
    }

    void SceneGraph::setAmbientLightColor(const math3d::vector4 & color) {
        ambientLight = color;
    }

    void SceneGraph::setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha) {
        ambientLight = math3d::vector4(red, green, blue, alpha);
    }

    const math3d::vector4 SceneGraph::getAmbientLightColor() const {
        return ambientLight;
    }
}
