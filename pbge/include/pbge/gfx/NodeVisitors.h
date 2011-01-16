

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
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

    protected:
        math3d::matrix44 transformationStack[UpdaterVisitor::MAX_STACK_DEPTH];

        int stackIndex;
    };

    class PBGE_EXPORT RenderVisitor {
    public:
        
    private:

    }

}


#endif