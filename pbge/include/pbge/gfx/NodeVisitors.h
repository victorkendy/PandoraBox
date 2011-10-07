

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

    template<typename Visitor>
    class VisitorTrait {
    public:
        typedef boost::function3<void, Visitor*, Node*, GraphicAPI*> VisitorAction;

        VisitorTrait(VisitorAction action, VisitorAction postAction) :
          visitorAction(action), visitorPostAction(postAction){}

        void visit(Visitor * visitor, Node * root, GraphicAPI * gfx) {
            visitorAction(visitor, root, gfx);
            std::vector<Node*>::iterator child;
            for(child = root->getChildren().begin(); child != root->getChildren().end(); child++)
                visit(visitor, *child, gfx);
            visitorPostAction(visitor, root, gfx);
        }
        void pushMatrix(const math3d::matrix44 & m) { stack.push(m); }
        void popMatrix() { stack.pop(); }
        void clearMatrixStack() { stack.clear(); }
        const math3d::matrix44 peekMatrix() { return stack.peek(); }
    private:
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
        RenderVisitor():trait(&RenderVisitor::visitAction,&RenderVisitor::postVisitAction){}

        void visit(Node * node, GraphicAPI * ogl){ 
            trait.clearMatrixStack();
            trait.visit(this, node, ogl); 
        }

        virtual void visitAction(Node * node, GraphicAPI * gfx) = 0;
        virtual void postVisitAction(Node * node, GraphicAPI * gfx) = 0;

        void pushTransformation(const math3d::matrix44 & m) {
            trait.pushMatrix(m);
        }

        void popTransformation() {
            trait.popMatrix();
        }

        const math3d::matrix44 getCurrentTransformation() {
            return trait.peekMatrix();
        }
    protected:
        VisitorTrait<RenderVisitor> trait;
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