

#ifndef PBGE_GFX_NODEVISITORS_H_
#define PBGE_GFX_NODEVISITORS_H_
#include <vector>

#include "math3d/math3d.h"

#include "pbge/core/core.h"

namespace pbge {

    class Node;
    class Light;
    class Camera;
    class OpenGL;
    class Shader;
    class GPUProgram;

    class PBGE_EXPORT UpdaterVisitor {
    public:
        static const int MAX_STACK_DEPTH = 16;

        UpdaterVisitor() {
            transformationStack = new math3d::matrix44[UpdaterVisitor::MAX_STACK_DEPTH];
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

        void addActiveLight(Light * light) {
            activeLights.push_back(light);
        }

        std::vector<Camera *> & getActiveCameras() {
            return activeCameras;
        }

    private:
        void _visit(Node * node, OpenGL * ogl);

        math3d::matrix44 * transformationStack;

        int stackIndex;

        std::vector<Camera *> activeCameras;

        std::vector<Light*> activeLights;
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

    class PBGE_EXPORT LightPassVisitor : public RenderVisitor {
    public:
        static Shader * getDefaultVertexShader();

        static Shader * getDefaultFragmentShader();

        static GPUProgram * getDefaultProgram();
    public:

        LightPassVisitor () {
            vertexShader = NULL;
            fragmentShader = NULL;
            gpuProgram = NULL;
        }

        Shader * getLightPassVertexShader() {
            if(vertexShader != NULL)
                return vertexShader;
            return LightPassVisitor::getDefaultVertexShader();
        }

        Shader * getLightPassFragmentShader() {
            if(fragmentShader != NULL)
                return fragmentShader;
            return LightPassVisitor::getDefaultFragmentShader();
        }

        GPUProgram * getLightPassProgram() {
            if(gpuProgram != NULL)
                return gpuProgram;
            return LightPassVisitor::getDefaultProgram();
        }

        void visit(Node * node, OpenGL * ogl);
    private:
        Shader * vertexShader;

        Shader * fragmentShader;

        GPUProgram * gpuProgram;
    };
}


#endif