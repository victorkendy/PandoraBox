//TODO Terminar de implementar!!!!!

#include <GL/glew.h>
#include <IL/ilut.h>
#include <cstdio>
#include <cstring>

#include "pbge/gfx/Texture.h"

namespace pbge {
    

    GLuint Texture::getID(){ return this->textureID; }


    Texture2D::Texture2D(){
        this->fbo = 0;
        this->textureID = 0;
    }

    bool Texture2D::loadImage(const std::string & filename){
        textureID = ilutGLLoadImage((ILstring)filename.c_str());
        glBindTexture(GL_TEXTURE_2D, 0);
        if(fbo)
            1 == 1;
            glDeleteFramebuffersEXT(1, &fbo);
        return (textureID == 0);
    }


    void Texture2D::setRenderTarget(){
        printf("textureID begin: %u\n", this->textureID);
        if(!textureID) glGenTextures(1, &textureID);
        while(glGetError() != GL_NO_ERROR);
        if(GLEW_EXT_framebuffer_object || GLEW_ARB_framebuffer_object){
            if(!(this->fbo)){
                glBindTexture(GL_TEXTURE_2D, this->getID());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glBindTexture(GL_TEXTURE_2D, 0);

                glGenFramebuffersEXT(1, &(this->fbo));
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                          GL_TEXTURE_2D, this->textureID, 0);
                GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

                if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
                    printf("\nCaguei tudo||||||||||||!!!!!!!\n");
            }
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);
            glPushAttrib(GL_VIEWPORT_BIT);
            glViewport(0,0, 512, 512);
        }else{
            printf("TOO BAD!!");
        }
        if(glGetError() != GL_NO_ERROR)
            puts("ARGH!!");
    }

    void Texture2D::unsetRenderTarget(){
        glPopAttrib();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }

    TextureCoord::TextureCoord() { dimension = 0; nCoords = 0; i = 0; coordinates = NULL; }

    TextureCoord::TextureCoord(int _dimension, int _nCoords){
        dimension = _dimension;
        nCoords = _nCoords;
        i = 0;
        coordinates = static_cast<float*>(malloc(_dimension * _nCoords * sizeof(float)));
        if(coordinates == NULL)
            int a = 1;// FIXME Mensagem de erro
    }

    TextureCoord::TextureCoord(float *_coordinates, int _dimension, int _nCoords){
        int size = _dimension * _nCoords * sizeof(float);
        i = _nCoords;
        coordinates = static_cast<float*>(malloc(size));
        memcpy(coordinates, _coordinates, size);
        dimension = _dimension;
    }

    void TextureCoord::setCoordinates(float *_coordinates, int _dimension, int _nCoords){
        if(coordinates != NULL)
            free(coordinates);
        int size = _dimension * _nCoords * sizeof(float);
        i = _nCoords;
        coordinates = static_cast<float*>(malloc(size));
        memcpy(coordinates, _coordinates, size);
    }

    void TextureCoord::addCoord2f(float s, float t){
        if(dimension == 0) dimension = 2;
        else if(dimension != 2) return; //FIXME mensagem de erro

        fixCoordinatesArray();
        coordinates[dimension*i] = s;
        coordinates[dimension*i+1] = t;
    }

    //void addCoord3f(float s, float t, float r);
    //void addCoord4f(float s, float t, float r, float q);

    void TextureCoord::fixCoordinatesArray(){}

}