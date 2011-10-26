#ifndef PBGE_COLLISION_BOUNDINGVOLUMES_H_
#define PBGE_COLLISION_BOUNDINGVOLUMES_H_
#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "math3d/math3d.h"

#include "pbge/core/core.h"

namespace pbge {
    class AABB;
    class BoundingFrustum;

    class PBGE_EXPORT BoundingFrustum {
    public:
        BoundingFrustum();

        /** updates the frustum planes.
        **/
        void setViewProjection(const math3d::matrix44 & vp);

        const math3d::vector4 getPlane(int i) {
            return planes[i];
        }
    private:
        boost::scoped_array<math3d::vector4> planes;
    };

    /** A Axis-aligned bounding box.

        @author Victor Kendy Harada
    **/
    class PBGE_EXPORT AABB {
    public:
        typedef enum {
            OBJECT_SPACE = 100,
            WORLD_SPACE
        } CoordinateSpace;

        AABB();

        AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz, CoordinateSpace space);
        
        /** changes the min and max values for the AABB. 
        **/
        void setDimensions(float minx, float miny, float minz, float maxx, float maxy, float maxz, CoordinateSpace space);

        /** Update the coordinate space of the AABB to world space.

            @todo implement
            @param worldTransform the object to world space matrix
        */
        void update(const math3d::matrix44 & worldTransform);

        /** Calculates the distance between the AABB and the point.

            @param point a vector of the form vector4(x,y,z,1.0f)
            @return the distance
        */
        float distance(const math3d::vector4 & point);

        /** calculates on result the AABB tha encloses this and other.
        **/
        void compose(AABB * other, AABB * result);

        /** Perform the frustum culling test
            @param bf a bounding box of a camera frustum
            @return true if there is an intersection with bf, false otherwise
        **/
        bool frustumCullingTest(BoundingFrustum * bf);
    private:
        boost::scoped_ptr<math3d::vector4> orMinCoord;
        boost::scoped_ptr<math3d::vector4> orMaxCoord;
        boost::scoped_ptr<math3d::vector4> minCoord;
        boost::scoped_ptr<math3d::vector4> maxCoord;
        boost::scoped_array<math3d::vector4> vertices;
        CoordinateSpace coordSpace;
    };
}

#endif