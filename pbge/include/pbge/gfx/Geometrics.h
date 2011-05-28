#ifndef PBGE_GFX_GEOMETRICS_H_
#define PBGE_GFX_GEOMETRICS_H_

#include "math3d/vector4.h"

#include "pbge/core/core.h"

namespace pbge {

    class VBOModel;
    class GraphicAPI;

    class PBGE_EXPORT Geometrics {
    public:
        /** Create a VBOModel with the vertices of a circunference

            @param radius The radius of the created circunference
            @param slices The subdivision that will be used
            @param gfx The implementation of a GraphicAPI
            @return The VBOModel representing the circunference
        */
        static VBOModel * createCircunference(float radius, unsigned slices, GraphicAPI * gfx);

        /** Create a VBOModel with the vertices of a sphere

            @param radius The radius of the created sphere
            @param slices The subdivision that will be used
            @param gfx The implementation of a GraphicAPI
            @return The VBOModel representing the sphere
        */
        static VBOModel * createSphere(float radius, unsigned slices, GraphicAPI * gfx);

        /** Creates a VBOModel with the vertices of a 3rd degree bezier curve

            @param p0 first control point
            @param p1 second control point
            @param p2 third control point
            @param p3 fourth control point
            @return The VBOModel representing the Bezier
        */
        static VBOModel * createBezier(const math3d::vector4 & p0, const math3d::vector4 & p1,
                                       const math3d::vector4 & p2, const math3d::vector4 & p3, 
                                       unsigned slices, GraphicAPI * gfx);

    };

}

#endif