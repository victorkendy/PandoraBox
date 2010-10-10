// TODO Refazer sem usar o ilut
#ifndef pbge_texture
#define pbge_texture 1

#include <GL/glew.h>
#include <string>
#include <vector>

#include "pbge/core/core.h"
#include "pbge/core/Object.h"
#include "pbge/gfx/RenderTarget.h"


#ifndef ILUT_USE_OPENGL
#define ILUT_USE_OPENGL 1
#endif


namespace pbge {

    class PBGE_EXPORT Texture{
    public:
        GLuint getID();
    protected:
        GLuint textureID;
        GLuint fbo;
    };


    class PBGE_EXPORT Texture2D: public Texture, public RenderTarget{
    public:
        Texture2D();

        bool loadImage(const std::string & filename);
        void setRenderTarget();
        void unsetRenderTarget();
    protected:
        GLuint wrap_s, wrap_t;
    };


    class PBGE_EXPORT TextureCoord: public Object{
    public:
        TextureCoord();
        TextureCoord(int _dimension, int _nCoords);
        TextureCoord(float *_coordinates, int _dimension, int _nCoords);
        void setCoordinates(float *_coordinates, int _dimension, int _nCoords);
        void addCoord2f(float s, float t);
        //void addCoord3f(float s, float t, float r);
        //void addCoord4f(float s, float t, float r, float q);
    protected:
        float *coordinates;
        int dimension;
        int nCoords;
        int i;
    private:
        void fixCoordinatesArray();
    };

}
#endif
