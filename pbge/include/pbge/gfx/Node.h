
#ifndef PBGE_GFX_NODE_H_
#define PBGE_GFX_NODE_H_

#include <iostream>
#include <string>
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/collision/BoundingVolumes.h"


namespace pbge {
    
    class GraphicAPI;
    class UpdaterVisitor;
    class RenderVisitor;
    class Camera;

    /* The expected interface of the scene graph node
     * the post*Pass are called when the visitor is returning
     * the purpose of those functions is to facilitate clean up
     * after the respective pass
    */
    class PBGE_EXPORT Node {
    public:
        Node() {
            sceneGraphIndex = -1;
        }

        typedef std::vector<Node *> node_list;
        
        virtual void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void renderPass(RenderVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void depthPass(RenderVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl) = 0;

        virtual void addChild(Node * node) = 0;
        
        virtual node_list & getChildren() = 0;

        int getSceneGraphIndex() {
            return sceneGraphIndex;
        }

        void setSceneGraphIndex(int index) {
            sceneGraphIndex = index;
        }

        virtual AABB * getBoundingVolume() {
            return NULL;
        }
    private:
        int sceneGraphIndex;
    };


    /* 
     * The node that represents a affine linear transformation 
     * on the scene graph
     */
    class PBGE_EXPORT TransformationNode : public Node {
    public: // factory methods
        static TransformationNode * translation(const float & x, const float & y, const float & z);

        static TransformationNode * scaling(const float & sx, const float & sy, const float & sz);

        static TransformationNode * rotation(const float & radAngle, const float & x, const float & y, const float & z);

    public: // instance methods
        TransformationNode() {
            this->transformation = new math3d::matrix44(math3d::identity44);
            this->current = new math3d::matrix44(math3d::identity44);
        }

        TransformationNode(const math3d::matrix44 & matrix) {
            this->transformation = new math3d::matrix44(matrix);
            this->current = new math3d::matrix44(math3d::identity44);
        }

        ~TransformationNode() {
            delete this->transformation;
            delete this->current;
        }

        void setTransformationMatrix(const math3d::matrix44 & m) {
            *transformation = m;
        }

        const math3d::matrix44 & getTransformationMatrix() {
            return *transformation;
        }

        void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl);

        void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl);

        void renderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void depthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void addChild(Node * node) {
            childs.push_back(node);
        }
        
        node_list & getChildren() {
            return childs;
        }
        
        // post multiply transformation composition

        TransformationNode * scale(const float & sx, const float & sy, const float & sz);

        TransformationNode * rotate(const float & radAngle, const float & x, const float & y, const float & z);

        TransformationNode * translate(const float & x, const float & y, const float & z);

    private:
        math3d::matrix44 * transformation;

        math3d::matrix44 * current;

        node_list childs;
    };


    /* 
     * Node that holds a camera
     * it is used to inform the renderer about active cameras
     */
    class PBGE_EXPORT CameraNode : public Node {
    public:
        CameraNode();

        CameraNode(Camera * _camera);

        void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl);

        void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl){}

        void renderPass(RenderVisitor * visitor, GraphicAPI * ogl){}

        void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl){}

        void depthPass(RenderVisitor * visitor, GraphicAPI * ogl){};

        void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl){};

        void lookAt(const math3d::vector4 & up, const math3d::vector4 & front);

        void setPerspective(const float & fovy, const float & aspect,
                             const float & near, const float & far);

        void setOrtho (const float & left, const float & right,
                       const float & bottom, const float & top,
                       const float & near, const float & far);

        void addChild(Node * node){
            childs.push_back(node);
        }
        
        node_list & getChildren() {
            return childs;
        }

        math3d::matrix44 & getViewTransformation() {
            return *viewTransformation;
        }

    private:
        math3d::matrix44 * viewTransformation;
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
