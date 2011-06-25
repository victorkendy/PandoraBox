

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "boost/function.hpp"
#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/gfx/MatrixStack.h"
#include "pbge/gfx/Node.h"

namespace pbge {

    class Light;
    class Camera;
    class GraphicAPI;

    /** A trait that encapsulates the graph traversal logic
        
        This trait performs a depth first search on the scene graph
        executing the defined action on every node.
    */
    template<typename Visitor>
    class VisitorTrait {
    public:
        VisitorTrait(boost::function3<void,Node*,Visitor*,GraphicAPI*> action1, boost::function3<void,Node*,Visitor*,GraphicAPI*> action2, boost::function2<void,Visitor*,GraphicAPI*> preAction) 
            : action(action1), after(action2), visitAction(preAction){}

        /** The depth first search logic

            @param node the node to be visited
            @param api  a instance of the graphic api
            @param visitor the visitor that is traversing the graph
        */
        void visit(Node * node, GraphicAPI * api, Visitor * visitor) {
            if(visitAction)
                visitAction(visitor, api);
            action(node, visitor, api);
            std::vector<Node*>::iterator child;
            for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
                visit(*child, api, visitor);
            after(node, visitor, api);
        }

        void pushMatrix(const math3d::matrix44 & m) { stack.push(m); }
        void popMatrix() { stack.pop(); }
        void clearMatrixStack() { stack.clear(); }
        const math3d::matrix44 peekMatrix() { return stack.peek(); }
    private:
        boost::function3<void,Node*,Visitor*,GraphicAPI*> action;
        boost::function3<void,Node*,Visitor*,GraphicAPI*> after;
        boost::function2<void,Visitor*,GraphicAPI*> visitAction;
        MatrixStack stack;
    };

    class PBGE_EXPORT UpdaterVisitor {
    public:
        UpdaterVisitor():trait(&Node::updatePass, &Node::postUpdatePass, NULL) {}

        ~UpdaterVisitor() {}

        void visit(Node * node, GraphicAPI * ogl);

        void pushTransformation(const math3d::matrix44 & m) {trait.pushMatrix(m);}
        void popTransformation() {trait.popMatrix();}
        const math3d::matrix44 getCurrentTransformation() {return trait.peekMatrix();}

        void addActiveCamera(Camera * camera) {
            activeCameras.push_back(camera);
        }

        void addActiveLight(Light * light) {
            activeLights.push_back(light);
        }

        std::vector<Light*> & getActiveLights() {
            return activeLights;
        }

        std::vector<Camera *> & getActiveCameras() {
            return activeCameras;
        }

    private:
        VisitorTrait<UpdaterVisitor> trait;

        std::vector<Camera *> activeCameras;

        std::vector<Light*> activeLights;
    };

    /* abstract class for a render pass visitor */
    class PBGE_EXPORT RenderVisitor {
    public:
        RenderVisitor(boost::function3<void,Node*,RenderVisitor*,GraphicAPI*> action1, boost::function3<void,Node*,RenderVisitor*,GraphicAPI*> action2):trait(action1, action2, &RenderVisitor::preVisit){}

        void visit(Node * node, GraphicAPI * ogl){ trait.visit(node, ogl, this); }

        void pushTransformation(const math3d::matrix44 & m) {
            trait.pushMatrix(m);
        }

        void popTransformation() {
            trait.popMatrix();
        }

        const math3d::matrix44 getCurrentTransformation() {
            return trait.peekMatrix();
        }
        virtual void preVisit(GraphicAPI* api){}

    protected:
        VisitorTrait<RenderVisitor> trait;
    };

    class PBGE_EXPORT ColorPassVisitor : public RenderVisitor {
    public:
        ColorPassVisitor():RenderVisitor(&Node::renderPass, &Node::postRenderPass) {}
    };

    class PBGE_EXPORT DepthPassVisitor : public RenderVisitor{
    public:
        DepthPassVisitor():RenderVisitor(&Node::depthPass, &Node::postDepthPass) {}
    };

    class PBGE_EXPORT LightPassVisitor : public RenderVisitor {
    public:
        LightPassVisitor():RenderVisitor(&Node::renderPass, &Node::postRenderPass) {
            currentLight = NULL;
        }

        void preVisit(GraphicAPI * api);

        void setCurrentLight(Light * light) {
            currentLight = light;
        }

        void setCurrentCamera(Camera * camera) {
            currentCamera = camera;
        }
    private:
        Camera * currentCamera;

        Light * currentLight;
    };
}


#endif