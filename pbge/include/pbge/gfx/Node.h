
#ifndef gfxnode_h_
#define gfxnode_h_

#include <iostream>
#include <string>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/core/Object.h"
#include "pbge/gfx/Model.h"

namespace pbge {
    
    class Node;

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
        typedef std::vector<ModelInstance *> model_list;

        Node(const std::string _name) { 
            name = new std::string(_name);
            childs = new node_list;
        }

        // Updates the node information 
        // Returns a pointer to the matrix transformation of the node
        virtual math3d::matrix44 * update(math3d::matrix44 * current);

        virtual math3d::matrix44 * getTransformationMatrix() {
            return &current;
        }
        
        // 
        virtual void accept(NodeVisitor * visitor) {
            visitor->visit(this);
        }

        /* Add a child to the node */
        void addChild(Node * node) {
            childs->push_back(node);
        }
        
        node_list * getChilds() {
            return childs;
        }

        void addModelInstance(ModelInstance * model) {
            models.push_back(model);
        }

        model_list * getModelList() {
            return &models;
        }

        // gets the name of the node
        std::string getName(){ return * name; }
    private:
        model_list models;
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
}
#endif
