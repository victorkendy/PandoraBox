
#ifndef PBGE_GFX_NODE_H_
#define PBGE_GFX_NODE_H_

#include <iostream>
#include <string>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/gfx/OpenGL.h"
#include "pbge/core/core.h"
#include "pbge/core/Object.h"


namespace pbge {
    
    class Node;
    class OpenGL;

    class PBGE_EXPORT NodeVisitor : public Object {
    public:

        // The function that will be called on the root of the subtree
        virtual void visit(Node * root);
    protected:

        // This function is called for every node of the tree
        virtual void _visit(Node * node);

        // Do the visitation routine
        virtual void doVisit(Node * node)=0;
    };

    class PBGE_EXPORT UpdaterVisitor : public NodeVisitor {
    public:
        void visit(Node * node) {
            currentTransform = NULL;
            NodeVisitor::visit(node);
        }

    protected:
        void _visit(Node * node) {
            math3d::matrix44 * _transform = currentTransform;
            NodeVisitor::_visit(node);
            currentTransform = _transform;
        }

        void doVisit(Node * node);
    private:
        math3d::matrix44 * currentTransform;
    };



    class PBGE_EXPORT Node : public Object {

    public:
        // A alias for the list of nodes
        typedef std::vector<Node *> node_list;
        
        Node() {
            this->name = new std::string("");
            this->childs = new node_list;
        }

        Node(const std::string _name) { 
            name = new std::string(_name);
            childs = new node_list;
        }

        // Updates the node information 
        // Returns a pointer to the matrix transformation of the node
        virtual math3d::matrix44 * update(math3d::matrix44 * current);

        virtual void render(OpenGL * ogl) = 0;

        virtual math3d::matrix44 * getTransformationMatrix() {
            return &current;
        }
        
        // 
        virtual void accept(NodeVisitor * visitor) {
            visitor->visit(this);
        }

        /* Add a child to the node */
        virtual void addChild(Node * node) {
            childs->push_back(node);
        }
        
        virtual node_list * getChilds() {
            return childs;
        }

        // gets the name of the node
        std::string getName(){ return * name; }
    private:
        node_list * childs;
        std::string * name;
    protected:
        math3d::matrix44 current;
    };


    class PBGE_EXPORT TransformationNode : public Node {
    public:
                
        TransformationNode() : Node(""){
            transformation = current = math3d::identity44;
        }

        TransformationNode(const std::string _name) : Node(_name) {
            transformation = current = math3d::identity44;
        }
        
        void setTransformationMatrix(math3d::matrix44 * m) {
            transformation = *m;
        }

        void render(OpenGL * ogl);

        math3d::matrix44 * update(math3d::matrix44 * m) {
            if(m != NULL)
                current = *m * transformation;
            else 
                current = transformation;
            return &current;
        }
    private:
        math3d::matrix44 transformation;
    };

    class StateProxy;

    class PBGE_EXPORT StateChangeNode : public Node {
    public:
        void enable(OpenGL::Mode mode);

        void disable(OpenGL::Mode mode);
    private:
        std::vector<StateProxy*> changes;
    };
}
#endif
