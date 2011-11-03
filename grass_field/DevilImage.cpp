#include <IL/il.h>
#include <iostream>
#include <stdexcept>
#include "DevilImage.h"

bool devilInitialized = false;
void initDevIL() {
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
        throw std::runtime_error("Unsupported DevIL version");
    if(!devilInitialized) {
        ilInit();
        ilEnable(IL_ORIGIN_SET);
        devilInitialized = true;
    }
}