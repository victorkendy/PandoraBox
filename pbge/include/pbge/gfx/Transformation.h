
#ifndef pbge_transform_h
#define pbge_transform_h

#include <GL/glew.h>

#include "pbge/core/core.h"
#include "pbge/core/Object.h"
#include "math3d/math3d.h"

namespace pbge {
    class PBGE_EXPORT TransformationSet: public Object{
    public:
        TransformationSet(){ this->loadIdentity(); }
        void loadIdentity();
        void rotate(float angle, float x, float y, float z);
        void translate(float dx, float dy, float dz);
        void scale(float s);
        const math3d::matrix44 getTransformationMatrix();
        void loadToOpenGL() const;
        void loadInverseToOpenGL() const;

        void rightCompose(const TransformationSet & t);
        void leftCompose(const TransformationSet & t);

        // Posteriormente colocar m�todo para carregar uma matriz que
        // representa uma concatena��o de escalas, transla��es e rota��es
        // para a representa��o interna
        TransformationSet & operator=(const TransformationSet &t);
    private:
        math3d::quaternion trans, rot;
        float s;
    };
}
#endif