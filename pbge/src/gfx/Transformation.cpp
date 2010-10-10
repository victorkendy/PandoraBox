#include <cmath>
#include <GL/glew.h>

#include "math3d/math3d.h"

#include "pbge/gfx/Transformation.h"
#include "pbge/core/definitions.h"

namespace pbge {

    void TransformationSet::loadIdentity(){
        s = 1.0f;
        rot = math3d::quaternion(1,0,0,0);
        trans = math3d::quaternion(0,0,0,0);
    }

    void TransformationSet::rotate(float angle, float x, float y, float z){
        float s = sin(angle/2);
        rot *= math3d::quaternion(cos(angle/2), s*x, s*y, s*z);
    }

    void TransformationSet::scale(float S){
        this->s *= S;
    }

    void TransformationSet::translate(float dx, float dy, float dz){
        math3d::quaternion q = this->rot * math3d::quaternion(0, dx, dy, dz) * this->rot.conjugate();
        q *= this->s;
        this->trans += q;
    }

    const math3d::matrix44 TransformationSet::getTransformationMatrix(){
        math3d::matrix44 m = this->rot.toMatrix();
        m *= this->s;
        m[3][3] = 1.0f;
        m[0][3] = this->trans[1];
        m[1][3] = this->trans[2];
        m[2][3] = this->trans[3];
        return m;
    }


    void TransformationSet::loadToOpenGL() const{
        GLfloat angle = 2 * acos(this->rot[0]);
        glTranslatef(this->trans[1], this->trans[2], this->trans[3]);
        if(angle >= PBGE_min_rotation && angle <= PBGE_max_rotation) {
            angle *= 180.0f * PBGE_1_pi;
            glRotatef(angle, this->rot[1], this->rot[2], this->rot[3]);
        }
        glScalef(this->s, this->s, this->s);
    }

    void TransformationSet::loadInverseToOpenGL() const{
        GLfloat angle = 2 * acos(this->rot[0]);
        GLfloat scale = 1.0f/this->s;
        glScalef(scale, scale, scale);
        if(angle >= PBGE_min_rotation && angle <= PBGE_max_rotation) {
            angle *= 180.0f * PBGE_1_pi;
            glRotatef(-angle, this->rot[1], this->rot[2], this->rot[3]);
        }
        glTranslatef(-this->trans[1], -this->trans[2], -this->trans[3]);
    }

    /* 
        É esperado que o resultado de f.compose(t); f.loadToOpenGL() seja
        o mesmo que f.loadToOpenGL(); t.loadToOpenGL();
    */
    void TransformationSet::rightCompose(const TransformationSet & t){
        this->translate(t.trans[1], t.trans[2], t.trans[3]);
        this->rot *= t.rot;
        this->s *= t.s;
    }

    void TransformationSet::leftCompose(const TransformationSet & t){
        TransformationSet aux = *this;
        *this = t;
        this->rightCompose(aux);
    }

    TransformationSet & TransformationSet::operator=(const TransformationSet &t){
        this->trans = t.trans;
        this->rot = t.rot;
        this->s = t.s;
        return *this;
    }
}