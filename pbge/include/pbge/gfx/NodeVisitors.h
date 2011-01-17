

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/core/Object.h"

namespace pbge {

    class Node;
    class OpenGL;
    class Camera;

    class PBGE_EXPORT UpdaterVisitor {
    public:
        static const int MAX_STACK_DEPTH = 16;

        UpdaterVisitor() {
            transformationStack[0] = math3d::identity44;
            stackIndex = 0;
        }
        
        void visit(Node * node, OpenGL * ogl);

        void pushTransformation(const math3d::matrix44 & m);

        void popTransformation();

        const math3d::matrix44 getCurrentTransformation();

        void addActiveCamera(Camera * camera) {
            activeCameras.push_back(camera);
        }

        std::vector<Camera *> & getActiveCameras() {
            return activeCameras;
        }

    protected:
        math3d::matrix44 transformationStack[UpdaterVisitor::MAX_STACK_DEPTH];
        int stackIndex;
        std::vector<Camera *> activeCameras;
    };

    
    /*
    class PBGE_EXPORT BasicRendererVisitor : public NodeVisitor {
    public:
        BasicRendererVisitor(OpenGL * _ogl) : ogl(_ogl){}
        virtual void visit(Node * node) {
            camera->setCamera(ogl);
            ogl->uploadProjection();
            NodeVisitor::visit(node);
            camera->unsetCamera(ogl);
        }

        virtual void doVisit(Node * node) {
            node->render(ogl);
        }

    */
    class PBGE_EXPORT RenderVisitor {
    public:
        void visit(Node * node, OpenGL * ogl);

        void setCurrentCamera(Camera * _camera) {
            camera = _camera;
        }
    private:
        void _visit(Node * node, OpenGL * ogl);
        Camera * camera;
    };

}


#endif