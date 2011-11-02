#include <IL/il.h>
#include <iostream>
#include "DevilImage.h"

bool devilInitialized = false;
void initDevIL() {
    std::cout << ilGetInteger(IL_VERSION_NUM) << " " << IL_VERSION << std::endl;
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
        throw 1;
    if(!devilInitialized) {
        ilInit();
        ilEnable(IL_ORIGIN_SET);
        devilInitialized = true;
    }
}