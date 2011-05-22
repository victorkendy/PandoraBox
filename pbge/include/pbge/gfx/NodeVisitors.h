

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/gfx/MatrixStack.h"

namespace pbge {

    class Node;
    class Light;
    class Camera;
    class GraphicAPI;

    class PBGE_EXPORT UpdaterVisitor {
    public:
        static const int MAX_STACK_DEPTH = 16;

        UpdaterVisitor() {
        }

        ~UpdaterVisitor() {
        }

        void visit(Node * node, GraphicAPI * ogl);

        void pushTransformation(const math3d::matrix44 & m);

        void popTransformation();

        const math3d::matrix44 getCurrentTransformation();

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
        void _visit(Node * node, GraphicAPI * ogl);

        MatrixStack stack;

        std::vector<Camera *> activeCameras;

        std::vector<Light*> activeLights;
    };

    /* abstract class for a render pass visitor */
    class PBGE_EXPORT RenderVisitor {
    public:
        virtual void visit(Node * node, GraphicAPI * ogl) = 0;

        void pushTransformation(const math3d::matrix44 & m) {
            stack.push(m);
        }

        void popTransformation() {
            stack.pop();
        }

        const math3d::matrix44 getCurrentTransformation() {
            return stack.peek();
        }
    private:
        MatrixStack stack;
    };

    class PBGE_EXPORT ColorPassVisitor : public RenderVisitor {
        void visit(Node * node, GraphicAPI * ogl);
    };

    class PBGE_EXPORT DepthPassVisitor : public RenderVisitor{
    public:
        void visit(Node * node, GraphicAPI * ogl);
    };

    class PBGE_EXPORT LightPassVisitor : public RenderVisitor {
    public:

        LightPassVisitor () {
            currentLight = NULL;
        }

        void setCurrentLight(Light * light) {
            currentLight = light;
        }

        void setCurrentCamera(Camera * camera) {
            currentCamera = camera;
        }

        void visit(Node * node, GraphicAPI * ogl);
    private:
        Camera * currentCamera;

        Light * currentLight;
    };
}


#endif