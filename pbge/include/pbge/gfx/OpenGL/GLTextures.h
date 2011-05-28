
#ifndef PBGE_GFX_OPENGL_GLTEXTURES_H_
#define PBGE_GFX_OPENGL_GLTEXTURES_H_

#include <GL/glew.h>

#include "pbge/gfx/Texture.h"

namespace pbge {
    class GraphicAPI;
    class Image;
    class TextureUnit;
    class GLObjectsFactory;
    class GLGraphic;

    class GLTexture1D : public Texture1D {
    public:
        GLTexture1D(GraphicAPI * gl);

        void setImageData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, Texture::Format internalFormat);

        void bindTextureOn(TextureUnit * unit);

        void setMinFilter(Texture::Filter minFilter);

        void setMagFilter(Texture::Filter magFilter);
    private:
        GraphicAPI * ogl;

        GLuint GLID;

        GLsizei width;

        GLenum dataType;

        GLenum dataFormat;

        GLvoid * data;

        GLenum internalFormat;

        GLenum minFilter;

        GLenum magFilter;
    };

    class GLTexture2D : public Texture2D {
    public:

        friend GLObjectsFactory;

        // check the documentation of the Texture2D interface
        void setImage(Image * img, Texture::Format format);

        // check the documentation of the Texture2D interface
        void setImageData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, int height, Texture::Format internalFormat);

        // check the documentation of the Texture interface
        void bindTextureOn(TextureUnit * unit);

        // check the documentation of the Texture interface
        void setMinFilter(Texture::Filter minFilter);

        // check the documentation of the Texture interface
        void setMagFilter(Texture::Filter magFilter);

    private:
        GLTexture2D (GraphicAPI * _gl); 
        
        // Checks if this texture object is compatible with the specified image and format
        // the image is compatible if the format equal the internalFormat and the dimensions
        // of the image is the same as the texture dimension
        bool isCompatibleWith(unsigned w, unsigned h, Texture::Format format);
        

        // If there is no GL object that corresponds to this instance yet, replace the internal 
        // temporary data and representation of the texture object
        void replaceInternalData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, int height, Texture::Format internalFormat);
        
        // If there is a GL object corresponding to this texture object then replace the data and 
        // representation of that object
        void replaceGLObjectData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, int height, Texture::Format internalFormat);

        GraphicAPI * ogl;

        GLuint GLID;

        GLsizei width, height;

        GLenum dataType;

        GLenum dataFormat;

        GLvoid * data;

        GLenum internalFormat;

        GLenum minFilter;

        GLenum magFilter;
    };
}

#endif
