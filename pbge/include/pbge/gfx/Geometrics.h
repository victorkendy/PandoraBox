#ifndef PBGE_GFX_GEOMETRICS_H_
#define PBGE_GFX_GEOMETRICS_H_

#include "pbge/core/core.h"

namespace pbge {

    class VBOModel;
    class GraphicAPI;

    class PBGE_EXPORT Geometrics {
    public:
        static VBOModel * createCircle(float radius, unsigned slices, GraphicAPI * gfx);
    };

}

#endif