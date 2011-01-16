

#ifndef PBGE_GFX_RENDERER_H_
#define PBGE_GFX_RENDERER_H_

#include <vector>
#include <GL/glew.h>

#include "pbge/core/Object.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Camera.h"

namespace pbge {
    class UpdaterVisitor;
    class RenderVisitor;

    class PBGE_EXPORT Renderer: public Object {
    public:
        Renderer(OpenGL * _ogl);

        void setScene(const SceneManager * scene_manager);

        SceneManager * getScene();

        void updateScene();

        void render();
    private:
        SceneManager * scene;

        UpdaterVisitor * updater;

        RenderVisitor * renderer;

        OpenGL * ogl;
    };
}
#endif
