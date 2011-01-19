

#ifndef PBGE_GFX_RENDERER_H_
#define PBGE_GFX_RENDERER_H_

#include <vector>
#include <GL/glew.h>

#include "pbge/core/core.h"
#include "pbge/core/Object.h"

namespace pbge {
    class UpdaterVisitor;
    class RenderVisitor;
    class Camera;
    class OpenGL;
    class SceneGraph;
    class Node;

    class PBGE_EXPORT Renderer: public Object {
    public:
        Renderer(OpenGL * _ogl);

        void setScene(const SceneGraph * scene_manager);

        SceneGraph * getScene();

        void updateScene();

        void render();
    private:
        void renderWithCamera(Camera * camera, Node * root);

        SceneGraph * scene;

        UpdaterVisitor * updater;

        RenderVisitor * renderer;

        RenderVisitor * depthRenderer;

        OpenGL * ogl;
    };
}
#endif
