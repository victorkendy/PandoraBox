

#ifdef _MSC_VER

#include <windows.h>
#include <cstdio>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


#include "math3d/math3d.h"

extern "C" {
    BOOL APIENTRY DllMain(HANDLE hModule, 
                          DWORD  ul_reason_for_call, 
                          LPVOID lpReserved) {

        switch(ul_reason_for_call) {
            case DLL_PROCESS_ATTACH:
                ilInit();
                //iluInit();
                //ilutInit();
                //math3d::getBestImplementation();
                break;
            case DLL_THREAD_ATTACH:
                break;
            case DLL_THREAD_DETACH:
                break;
            case DLL_PROCESS_DETACH:
                break;
        }
        return TRUE;
    }
}

#endif