
#ifndef gfxsnode_h_
#define gfxsnode_h_

#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"

namespace pbge {
    class Node;

    class PBGE_EXPORT SceneGraph{
    public:
        /** The constant representing the root node of the scene graph
        */
        static const int ROOT = 0;

        /** SceneGraph constructor

            @param node The scene graph root node
        */
        SceneGraph(Node * node) {
            nodes.push_back(node);
        }

        ~SceneGraph();

        Node * getSceneGraphRoot() {
            return nodes.at(0);
        }

        /** Append a new child to a scene graph node
            
            @param index The index of the parent node on the scene graph
            @param child The new child of the node identified by index
            @return The appended node
        */
        Node * appendChildTo(int index, Node * child);

        /** Append a new child to a scene graph node
            
            Same as appendChildTo(parent->getSceneGraphIndex(), child)

            @param parent The parent
            @param child The new child of parent
            @return The appended node

        */
        Node * appendChildTo(Node * parent, Node * child);

        /** Get a identified graph node
            
            @param index The id of the node on the scene graph
            @return The node with the given index
        */
        Node * getGraphNode(int index);

        void setAmbientLightColor(const math3d::vector4 & color);

        void setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha);

        const math3d::vector4 getAmbientLightColor() const;
    private:
        math3d::vector4 ambientLight;
        std::vector<Node*> nodes;
    };
}
#endif

