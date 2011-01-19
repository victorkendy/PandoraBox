

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/core/Object.h"

namespace pbge {

    class Node;
    class OpenGL;

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

    /* Interface for a visitor class that render something on some OpenGL buffer */
    class PBGE_EXPORT RenderVisitor {
    public:
        virtual void visit(Node * node, OpenGL * ogl) = 0;
    };

    class PBGE_EXPORT ColorPassVisitor : public RenderVisitor {
        void visit(Node * node, OpenGL * ogl);
    };

    class PBGE_EXPORT DepthPassVisitor : public RenderVisitor{
    public:
        void visit(Node * node, OpenGL * ogl);
    };

}


#endif