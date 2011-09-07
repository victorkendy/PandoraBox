#include <GL/glew.h>

#include "math3d/math3d.h"

#include "pbge/gfx/GraphicAPI.h"
#include "OpenGLAPI/gfx/BuiltInUniformBinders.h"

using namespace pbge;

void DeprModelViewBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 modelView = (gfx->getViewMatrix() * gfx->getModelMatrix()).transpose();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelView);
}

void DeprProjectionBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 projection = gfx->getProjectionMatrix().transpose();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

void DeprModelViewProjectionBinder::bind(pbge::GraphicAPI *gfx) {
    math3d::matrix44 projection = gfx->getProjectionMatrix().transpose();
    math3d::matrix44 modelView = (gfx->getViewMatrix() * gfx->getModelMatrix()).transpose();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelView);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

class ModelMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        return gfx->getModelMatrix();
    }
};
class ViewMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        return gfx->getViewMatrix();
    }
};
class ProjectionMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        return gfx->getProjectionMatrix();
    }
};
class ModelViewMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        return gfx->getViewMatrix() * gfx->getModelMatrix();
    }
};
class ModelViewProjectionMatrixGetter {
public:
    const math3d::matrix44 operator() (GraphicAPI* gfx) {
        return gfx->getProjectionMatrix() * gfx->getViewMatrix() * gfx->getModelMatrix();
    }
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