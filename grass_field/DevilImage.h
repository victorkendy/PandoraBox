#include <string>
#include <cstring>
#include <IL/il.h>
#include <cstdlib>
#include "pbge/pbge.h"

#ifndef GRASS_FIELD_DEVILIMAGE_H_
#define GRASS_FIELD_DEVILIMAGE_H_

void initDevIL();

class DevilImage: public pbge::Image {
public:
    explicit DevilImage(const std::string & filePath) {
        initDevIL();
        ilGenImages(1, &imageid);
        ilBindImage(imageid);
        if(!ilLoadImage((const ILstring)filePath.c_str())) {
            throw 1;
        }
        convertImage();
        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        // width * height * number of color channels
        dataSize = width * height * 4;
        data = (char*)malloc(dataSize);
        memcpy(data, ilGetData(), dataSize);
        ilDeleteImages(1, &imageid);
    }
    unsigned getHeight() {
        return height;
    }
    unsigned getWidth() {
        return width;
    }
    pbge::Texture::Format getFormat() {
        return pbge::Texture::RGBA;
    }
    pbge::Texture::DataType getDataType() {
        return pbge::Texture::UNSIGNED_BYTE;
    }
    size_t getDataSize() {
        return dataSize;
    }
    void * getData() { 
        return data;
    }
    ~DevilImage() {
        free(data);
    }
private:
    void convertImage() {
        if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
            throw 1;
        }
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    }
    ILuint imageid;
    unsigned width;
    unsigned height;
    char * data;
    size_t dataSize;
};

#endif