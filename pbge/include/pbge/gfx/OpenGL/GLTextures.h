
#ifndef PBGE_GFX_OPENGL_GLTEXTURES_H_
#define PBGE_GFX_OPENGL_GLTEXTURES_H_

#include <GL/glew.h>

#include "pbge/gfx/Texture.h"

namespace pbge {
    class OpenGL;
    class Image;
    class TextureUnit;

    class GLTexture2D : Texture2D {

    friend OpenGL;

    public:
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
        GLTexture2D (OpenGL * _gl); 
        
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

        OpenGL * ogl;

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
