

#ifndef PBGE_GFX_IMAGE_H_
#define PBGE_GFX_IMAGE_H_

#include "pbge/gfx/Texture.h"

namespace pbge {
    class Image {
    public:
        virtual Texture::DataType getDataType() = 0;

        virtual Texture::Format getFormat() = 0;

        virtual size_t getDataSize() = 0;

        virtual void * getData() = 0;

        virtual unsigned getWidth() = 0;

        virtual unsigned getHeight() = 0;
    };
}


#endif