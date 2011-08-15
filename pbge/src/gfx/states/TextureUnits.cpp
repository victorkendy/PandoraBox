
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/states/TextureUnits.h"

using namespace pbge;

TextureUnit::TextureUnit(unsigned _index) {
    this->nextTexture = NULL;
    this->boundTexture = NULL;
    this->index = _index;
}

bool TextureUnit::shouldChange(GraphicAPI * ogl) {
    return boundTexture != nextTexture;
}

void TextureUnit::makeChange(GraphicAPI * ogl) {
    if(nextTexture == NULL && boundTexture != NULL) {
        // should unbind texture?
    } else {
        nextTexture->bindTextureOn(this);
    }
    boundTexture = nextTexture;
}



TextureUnits::TextureUnits(GraphicAPI * api) {
    int numberOfUnits = api->numberOfTextureUnits();
    units.reserve(numberOfUnits);
    for(int i = 0; i < numberOfUnits; i++) {
        units.push_back(TextureUnit(i));
    }
}

bool TextureUnits::shouldChange(GraphicAPI *ogl) {
    return true;
}

void TextureUnits::makeChange(GraphicAPI *ogl) {
}

TextureUnit * TextureUnits::unitFor(Texture *texture) {
    std::vector<TextureUnit>::iterator unit;
    TextureUnit * chosen = NULL;
    for(unit = units.begin(); unit != units.end(); unit++) {
        if(unit->getCurrentTexture() == texture) {
            chosen = &(*unit);
            break;
        } else if((unit->getCurrentTexture() == NULL) && (chosen == NULL))
            chosen = &(*unit);
    }
    return chosen;
}