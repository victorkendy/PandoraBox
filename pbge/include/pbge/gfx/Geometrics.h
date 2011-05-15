#ifndef PBGE_GFX_GEOMETRICS_H_
#define PBGE_GFX_GEOMETRICS_H_

#include "math3d/vector4.h"

#include "pbge/core/core.h"

namespace pbge {

    class VBOModel;
    class GraphicAPI;

    class PBGE_EXPORT Geometrics {
    public:
        static VBOModel * createCircunference(float radius, unsigned slices, GraphicAPI * gfx);

        static VBOModel * createBezier(const math3d::vector4 & p0, const math3d::vector4 & p1,
                                       const math3d::vector4 & p2, const math3d::vector4 & p3, 
                                       unsigned slices, GraphicAPI * gfx);
    };

}

#endif