
#ifndef PBGE_GFX_NODE_H_
#define PBGE_GFX_NODE_H_

#include <iostream>
#include <string>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/core/Object.h"


namespace pbge {
    
    class OpenGL;
    class UpdaterVisitor;
    class Camera;

    // The expected interface of the scene graph node
    class PBGE_EXPORT Node : public Object {
    public:
        typedef std::vector<Node *> node_list;
        
        virtual void updatePass(UpdaterVisitor * visitor, OpenGL * ogl) = 0;

        virtual void postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl) = 0;

        //virtual void renderPass(NodeVisitor * visitor, OpenGL * ogl) = 0;

        //virtual void postRenderPass(NodeVisitor * visitor, OpenGL * ogl) = 0;

        virtual void addChild(Node * node) = 0;
        
        virtual node_list & getChilds() = 0;
    };


    class PBGE_EXPORT TransformationNode : public Node {
    public:
        TransformationNode() {
            this->transformation = math3d::identity44;
        }

        void setTransformationMatrix(const math3d::matrix44 & m) {
            transformation = m;
        }

        void updatePass(UpdaterVisitor * visitor, OpenGL * ogl);

        void postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl);

        //void renderPass(NodeVisitor * visitor, OpenGL * ogl);

        //void postRenderPass(NodeVisitor * visitor, OpenGL * ogl) {}

        void addChild(Node * node) {
            childs.push_back(node);
        }
        
        node_list & getChilds() {
            return childs;
        }

    private:
        node_list childs;

        math3d::matrix44 transformation;

        math3d::matrix44 current;
    };

    class PBGE_EXPORT CameraNode : public Node {
    public:
        CameraNode(Camera * _camera) {
            viewTransformation = math3d::identity44;
            this->camera = _camera;
        }
        virtual void updatePass(UpdaterVisitor * visitor, OpenGL * ogl);

        virtual void postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl){}

        //virtual void renderPass(NodeVisitor * visitor, OpenGL * ogl) = 0;

        //virtual void postRenderPass(NodeVisitor * visitor, OpenGL * ogl) = 0;

        void addChild(Node * node){
            childs.push_back(node);
        }
        
        node_list & getChilds() {
            return childs;
        }

        math3d::matrix44 & getViewTransformation() {
            return viewTransformation;
        }

    private:
        math3d::matrix44 viewTransformation;
        Camera * camera;
        node_list childs;
    };
/*
    class StateProxy;
    class PBGE_EXPORT StateChangeNode : public Node {
    public:
        void enable(OpenGL::Mode mode);

        void disable(OpenGL::Mode mode);
    private:
        std::vector<StateProxy*> changes;
    };
*/
}
#endif
