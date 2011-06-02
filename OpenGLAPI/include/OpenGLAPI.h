#ifndef OPENGLAPI_OPENGL_H_
#define OPENGLAPI_OPENGL_H_

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
extern "C" {
__declspec(dllexport) pbge::GraphicAPI * createInstance();
}
#endif
