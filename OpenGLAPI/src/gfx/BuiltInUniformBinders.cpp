#include <GL/glew.h>

#include "math3d/math3d.h"

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/ShaderUniform.h"
#include "OpenGLAPI/gfx/BuiltInUniformBinders.h"


using namespace pbge;

void DeprModelViewBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 modelView = (gfx->getViewMatrix()->get() * gfx->getModelMatrix()->get()).transpose();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelView);
}

void DeprProjectionBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 projection = gfx->getProjectionMatrix()->get().transpose();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

void DeprModelViewProjectionBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 projection = gfx->getProjectionMatrix()->get().transpose();
    math3d::matrix44 modelView = (gfx->getViewMatrix()->get() * gfx->getModelMatrix()->get()).transpose();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelView);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

class ModelMatrixGetter {
public:
    ModelMatrixGetter():stamp(0){}
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        stamp = gfx->getModelMatrix()->getStamp();
        return gfx->getModelMatrix()->get();
    }
    bool shouldChange(GraphicAPI * gfx) {
        return stamp != gfx->getModelMatrix()->getStamp();
    }
private:
    unsigned long stamp;
};
class ViewMatrixGetter {
public:
    ViewMatrixGetter():stamp(0){}
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        stamp = gfx->getViewMatrix()->getStamp();
        return gfx->getViewMatrix()->get();
    }
    bool shouldChange(GraphicAPI * gfx) {
        return stamp != gfx->getViewMatrix()->getStamp();
    }
private:
    unsigned long stamp;
};
class ProjectionMatrixGetter {
public:
    ProjectionMatrixGetter():stamp(0){}
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        stamp = gfx->getProjectionMatrix()->getStamp();
        return gfx->getProjectionMatrix()->get();
    }
    bool shouldChange(GraphicAPI * gfx) {
        return stamp != gfx->getProjectionMatrix()->getStamp();
    }
private:
    unsigned long stamp;
};
class ModelViewMatrixGetter {
public:
    ModelViewMatrixGetter():modelStamp(0),viewStamp(0){}
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        viewStamp = gfx->getViewMatrix()->getStamp();
        modelStamp = gfx->getModelMatrix()->getStamp();
        math3d::matrix44 view = gfx->getViewMatrix()->get();
        math3d::matrix44 model = gfx->getModelMatrix()->get();
        return view * model;
    }
    bool shouldChange(GraphicAPI * gfx) {
        return viewStamp != gfx->getViewMatrix()->getStamp() ||
               modelStamp != gfx->getModelMatrix()->getStamp();
    }
private:
    unsigned long viewStamp, modelStamp;
};
class ModelViewProjectionMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        viewStamp = gfx->getViewMatrix()->getStamp();
        modelStamp = gfx->getModelMatrix()->getStamp();
        projStamp = gfx->getProjectionMatrix()->getStamp();
        math3d::matrix44 view = gfx->getViewMatrix()->get();
        math3d::matrix44 model = gfx->getModelMatrix()->get();
        math3d::matrix44 projection = gfx->getProjectionMatrix()->get();
        return projection * view * model;
    }
    bool shouldChange(GraphicAPI * gfx) {
        return viewStamp != gfx->getViewMatrix()->getStamp() ||
               modelStamp != gfx->getModelMatrix()->getStamp() ||
               projStamp != gfx->getProjectionMatrix()->getStamp();
    }
private:
    unsigned long viewStamp, modelStamp, projStamp;
};


BuiltInUniformBinder * BuiltInUniformBinders::binderFor(const std::string &name, GLint location) {
    if(location == -1) { // probably built-in
        if(name == "gl_ModelViewMatrix") {
            return new DeprModelViewBinder;
        }else if (name == "gl_ModelViewProjectionMatrix") {
            return new DeprModelViewProjectionBinder;
        } else if (name == "gl_ProjectionMatrix") {
            return new DeprProjectionBinder;
        } else {
            return NULL;
        }
    } else { // pbge's built-in
        if(name == "pbge_ModelMatrix") {
            return new MatrixBinder<ModelMatrixGetter>(location);
        } else if(name == "pbge_ViewMatrix") {
            return new MatrixBinder<ViewMatrixGetter>(location);
        } else if(name == "pbge_ProjectionMatrix") {
            return new MatrixBinder<ProjectionMatrixGetter>(location);
        } else if(name == "pbge_ModelViewMatrix") {
            return new MatrixBinder<ModelViewMatrixGetter>(location);
        } else if(name == "pbge_ModelViewProjectionMatrix") {
            return new MatrixBinder<ModelViewProjectionMatrixGetter>(location);
        } else {
            return NULL;
        }
    }
}