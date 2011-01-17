
#ifndef gfxsnode_h_
#define gfxsnode_h_

#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/core/Object.h"

namespace pbge {
    class Node;

    class PBGE_EXPORT SceneGraph: public Object{
    public:
        static const int ROOT = 0;

        SceneGraph(Node * node) {
            nodes.push_back(node);
            ambientLight = math3d::vector4(0,0,0,0);
        }

        Node * getSceneGraphRoot() {
            return nodes.at(0);
        }

        Node * appendChildTo(int index, Node * child);

        Node * appendChildTo(Node * parent, Node * child);

        void setAmbientLightColor(const math3d::vector4 & color);

        void setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha);

        const math3d::vector4 getAmbientLightColor() const;
    private:
        math3d::vector4 ambientLight;
        std::vector<Node*> nodes;
    };
}
#endif

