

#ifndef PBGE_GFX_IMAGE_H_
#define PBGE_GFX_IMAGE_H_

#include <cstddef>
#include "pbge/gfx/Texture.h"

namespace pbge {
    class Image {
    public:
        /**
            @return The data type of the image
        */
        virtual Texture::DataType getDataType() = 0;

        /**
            @return The pixel format
        */
        virtual Texture::Format getFormat() = 0;

        /**
            @return the size of the raw data in bytes
        */
        virtual size_t getDataSize() = 0;

        /**
            @return The raw data of the image
        */
        virtual void * getData() = 0;

        /**
            @return The width of the image
        */
        virtual unsigned getWidth() = 0;

        /**
            @return The height of the image
        */
        virtual unsigned getHeight() = 0;
    };
}


#endif