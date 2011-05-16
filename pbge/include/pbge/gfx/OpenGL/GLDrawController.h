#ifndef PBGE_GFX_OPENGL_GLDRAWCONTROLLER_H_
#define PBGE_GFX_OPENGL_GLDRAWCONTROLLER_H_

#include "pbge/gfx/DrawController.h"

namespace pbge {
    class Model;
    class VBOModel;
    class GLGraphic;

    class GLDrawController : public DrawController {
    public:
        GLDrawController(GLGraphic * ogl);

        void draw(Model * model);

        void draw(Model * model, int times);

        void drawVBOModel(VBOModel * model);

        void drawVBOModel(VBOModel * model, int times);

        void initialize();

    private:

        void bindVBO(VertexBuffer * buffer);

        void unbindVBO(VertexBuffer * buffer);

        void callRender(Model * model);

        GLGraphic * ogl;

        bool coreSupported;

        bool arbSupported;

        bool extSupported;
    };

}

#endif