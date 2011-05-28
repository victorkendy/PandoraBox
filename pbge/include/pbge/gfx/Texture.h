#ifndef PBGE_GFX_TEXTURE_H_
#define PBGE_GFX_TEXTURE_H_

#include <string>
#include <vector>

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"

namespace pbge {
    class Image;

    /** The interface that all textures must implement
        
        @author Victor Kendy Harada
    */
    class Texture {
    public:
        /** The allowed values for the texture internal format and data format
            @enum Format
        */
        typedef enum {
            COMPRESSED_RED = GL_COMPRESSED_RED,
            COMPRESSED_RG = GL_COMPRESSED_RG,
            COMPRESSED_RGB = GL_COMPRESSED_RGB,
            COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
            COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
            COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA,
            DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
            DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
            DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
            DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
            R3_G3_B2 = GL_R3_G3_B2,
            RED = GL_RED,
            RG = GL_RG,
            RGB = GL_RGB,
            RGB4 = GL_RGB4,
            RGB5 = GL_RGB5,
            RGB8 = GL_RGB8,
            RGB10 = GL_RGB10,
            RGB12 = GL_RGB12,
            RGB16 = GL_RGB16,
            RGBA = GL_RGBA,
            RGBA2 = GL_RGBA2,
            RGBA4 = GL_RGBA4,
            RGBA5_A1 = GL_RGB5_A1,
            RGBA8 = GL_RGBA8,
            RGB10_A2 = GL_RGB10_A2,
            RGBA12 = GL_RGBA12,
            RGBA16 = GL_RGBA16,
            SRGB = GL_SRGB,
            SRGB8 = GL_SRGB8,
            SRGB_ALPHA = GL_SRGB_ALPHA,
            SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,

            BGR = GL_BGR,
            BGRA = GL_BGRA
        } Format;

        
        /** The allowed texture filtering functions
            @todo Add NEAREST, ROUND and CLAMP filter types
            @enum Filter
        */
        typedef enum {
            LINEAR = GL_LINEAR,
            NEAREST = GL_NEAREST
        } Filter;

        /** The allowed data types for the image raw data
            @enum DataType
        */
        typedef enum {
            UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
            BYTE = GL_BYTE,
            BITMAP = GL_BITMAP,
            UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
            SHORT = GL_SHORT,
            UNSIGNED_INT = GL_UNSIGNED_INT,
            INT = GL_INT,
            FLOAT = GL_FLOAT,
            UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
            UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
            UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
            UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
            UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
            UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
            UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
            UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
            UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
            UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
            UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
            UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
        } DataType;

        // Interface Definition
        
        // Changes the texture minification filter
        // default is Linear
        virtual void setMinFilter(Filter filter) = 0;

        // Changes the texture magnification filter
        // default is Linear
        virtual void setMagFilter(Filter filter) = 0;
        
        // binds the texture object to a given texture unit
        virtual void bindTextureOn(TextureUnit * unit) = 0;
    };

    class Texture1D : public Texture {
    public:
        /** Replace the texture data with the given raw data obeying the specified format.
            
            @param type The representation of the image data
            @param dataFormat The type of the image data
            @param image The raw image data
            @param size The size of the image data in bytes
            @param width The number of pixels of the image data
            @param internalFormat The desired internal representation that should be used on the graphic driver
        */
        virtual void setImageData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, Texture::Format internalFormat) = 0;
    };

    class Texture2D : public Texture {
    public:
        /** replace the internal image of the texture object with given image loader implementation.
            
            @param image an implementation of the image interface
            @param format the desired internal representation of the texture on the graphic api
        */
        virtual void setImage(Image * image, Texture::Format format) = 0;
        
        /** Replace the texture data with the given raw data obeying the specified format.

            @param type
            @param dataFormat
            @param image
            @param size
            @param width
            @param height
            @param internalFormat
        */
        virtual void setImageData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, int width, int height, Texture::Format internalFormat) = 0;
    };        
}
#endif
