

#ifndef PBGE_GFX_OPENGL_GLEXTENSIONS_H
#define PBGE_GFX_OPENGL_GLEXTENSIONS_H

#include <GL/glew.h>

namespace pbge {
    class GLGraphic;

    class TextureBufferInfo {
    public:
        TextureBufferInfo(GLGraphic * ogl);
        /** GL_XXX_texture_buffer_object extended texture binding point */
        GLenum binding;
        GLenum bufferBinding;
    };

    class TextureFormats {
    public:
        TextureFormats(GLGraphic * ogl);

        /** GL_XXX_texture_float defined formats */
        GLint rgba32f;
        GLint rgb32f;
        GLint rg32f;
        GLint r32f;
    };

    class GLExtensions {
    public:
        GLExtensions(GLGraphic * ogl);

        const TextureBufferInfo & getBufferTextureInfo() const {
            return bufferTextureInfo;
        }

        const TextureFormats & getTextureFormats() const {
            return textureFormats;
        }

    private:
        TextureBufferInfo bufferTextureInfo;

        TextureFormats textureFormats;
    };
}
#endif
