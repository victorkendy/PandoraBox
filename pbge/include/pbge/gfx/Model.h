

#ifndef gfxmodel_h_
#define gfxmodel_h_

#include "pbge/core/core.h"
#include "pbge/core/Object.h"
#include "pbge/gfx/OpenGL.h"

namespace pbge {
    class VertexBuffer;
    class ModelInstance;

    class PBGE_EXPORT Model : public Object {
    public:
        virtual void render(ModelInstance * instance, OpenGL * ogl)=0;
    };

    class PBGE_EXPORT VBOModel : public Model {
    public:
        VBOModel(VertexBuffer * vbo, GLenum primitive);

        void render(ModelInstance * instance, OpenGL * ogl);
    private:
        VertexBuffer * vbo;
        GLenum primitive;
    };


    class PBGE_EXPORT ModelInstance : public Object {
    public:
        ModelInstance(Model * _model) {
            model = _model;
        }
        
        virtual void render(OpenGL * ogl) {
            model->render(this, ogl);
        }

        Model * getModel() {
            return model;
        }
    private:
        Model * model;
    };
}
#endif
