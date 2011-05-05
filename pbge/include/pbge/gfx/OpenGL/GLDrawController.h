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

        
    private:
        void callRender(Model * model);

        void drawVBO(VBOModel * model);

        void drawVBO(VBOModel * model, int times);

        GLGraphic * ogl;
    };

}

#endif