

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "boost/function.hpp"
#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/collision/BoundingVolumes.h"
#include "pbge/gfx/Camera.h"
#include "pbge/gfx/MatrixStack.h"
#include "pbge/gfx/Node.h"

namespace pbge {
    class Light;
    class Camera;
    class GraphicAPI;

    template<typename Visitor, bool frustumCulled=false>
    class VisitorTrait {
    public:
        typedef boost::function3<void, Visitor*, Node*, GraphicAPI*> VisitorAction;

        VisitorTrait(VisitorAction action, VisitorAction postAction) :
          visitorAction(action), visitorPostAction(postAction){}

        void visit(Visitor * visitor, Node * root, GraphicAPI * gfx) {
            if(!frustumCulled) {
                doVisit(visitor, root, gfx);
            } else {
                doVisit(visitor, root, gfx);
            }
        }
        
        void pushMatrix(const math3d::matrix44 & m) { stack.push(m); }
        void popMatrix() { stack.pop(); }
        void clearMatrixStack() { stack.clear(); }
        const math3d::matrix44 peekMatrix() { return stack.peek(); }
    private:
        void doVisit(Visitor * visitor, Node * root, GraphicAPI * gfx) {
            visitorAction(visitor, root, gfx);
            std::vector<Node*>::iterator child;
            for(child = root->getChildren().begin(); child != root->getChildren().end(); child++)
                visit(visitor, *child, gfx);
            visitorPostAction(visitor, root, gfx);
        }
        MatrixStack stack;
        VisitorAction visitorAction;
        VisitorAction visitorPostAction;
    };

    class PBGE_EXPORT UpdaterVisitor {
    public:
        UpdaterVisitor():trait(&UpdaterVisitor::visitAction, &UpdaterVisitor::postVisitAction) {}

        void visit(Node * node, GraphicAPI * ogl);

        void visitAction(Node * node, GraphicAPI * gfx) {
            node->updatePass(this, gfx);
        }

        void postVisitAction(Node * node, GraphicAPI * gfx) {
            node->postUpdatePass(this, gfx);
            if(node->getBoundingVolume() != NULL) {
                node->getBoundingVolume()->update(getCurrentTransformation());
            }
        }

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
        RenderVisitor(){}

        void visit(Node * node, GraphicAPI * gfx){ 
            stack.clear();
            boundingFrustum = currentCamera->getBoundingFrustum();
            dfsVisit(node, gfx);
        }

        void setCurrenCamera(Camera * camera) {
            currentCamera = camera;
        }

        virtual void visitAction(Node * node, GraphicAPI * gfx) = 0;
        virtual void postVisitAction(Node * node, GraphicAPI * gfx) = 0;

        void pushTransformation(const math3d::matrix44 & m) {
            stack.push(m);
        }

        void popTransformation() {
            stack.pop();
        }

        const math3d::matrix44 getCurrentTransformation() {
            return stack.peek();
        }
    protected:
        void dfsVisit(Node * node, GraphicAPI * gfx) {
            if(node->getBoundingVolume() == NULL || node->getBoundingVolume()->frustumCullingTest(boundingFrustum)) {
                visitAction(node, gfx);
                std::vector<Node*>::iterator child;
                for(child = node->getChildren().begin(); child != node->getChildren().end(); child++)
                    dfsVisit(*child, gfx);
                postVisitAction(node, gfx);
            }
        }
        MatrixStack stack;
        Camera * currentCamera;
        BoundingFrustum * boundingFrustum;
    };

    class PBGE_EXPORT ColorPassVisitor : public RenderVisitor {
    public:
        void visitAction(Node * node, GraphicAPI * gfx) {
            node->renderPass(this, gfx);
        }
        void postVisitAction(Node * node, GraphicAPI * gfx) {
            node->postRenderPass(this, gfx);
        }
    };
}


#endif