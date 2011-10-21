#include "pbge/collision/BoundingVolumes.h"

using namespace pbge;

BoundingFrustum::BoundingFrustum() {
    math3d::vector4 * vs = new math3d::vector4[6];
    planes.reset(vs);
}

// Frustum plane extraction described by Gil Gribb and Klaus Hartmann
void BoundingFrustum::setViewProjection(const math3d::matrix44 & vp) {
    // left plane
    for(int i = 0; i < 4; i++)
        planes[0][i] = vp[3][i] + vp[0][i];
    // right plane
    for(int i = 0; i < 4; i++)
        planes[1][i] = vp[3][i] - vp[0][i];
    // bottom plane
    for(int i = 0; i < 4; i++)
        planes[2][i] = vp[3][i] + vp[1][i];
    // top plane
    for(int i = 0; i < 4; i++)
        planes[3][i] = vp[3][i] - vp[1][i];
    // near plane
    for(int i = 0; i < 4; i++)
        planes[4][i] = vp[3][i] + vp[2][i];
    // far plane
    for(int i = 0; i < 4; i++)
        planes[5][i] = vp[3][i] - vp[2][i];
}

AABB::AABB():
orMinCoord(new math3d::vector4),
orMaxCoord(new math3d::vector4),
minCoord(new math3d::vector4),
maxCoord(new math3d::vector4){}

AABB::AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz, AABB::CoordinateSpace space):
orMinCoord(new math3d::vector4),
orMaxCoord(new math3d::vector4),
minCoord(new math3d::vector4),
maxCoord(new math3d::vector4) {
    setDimensions(minx, miny, minz, maxx, maxy, maxz, space);
}

void AABB::setDimensions(float minx, float miny, float minz, float maxx, float maxy, float maxz, AABB::CoordinateSpace space) {
    *orMinCoord = math3d::vector4(minx, miny, minz, 1.0f);
    *orMaxCoord = math3d::vector4(maxx, maxy, maxz, 1.0f);
    coordSpace = space;
    if(coordSpace == AABB::WORLD_SPACE) {
        *minCoord = *orMinCoord;
        *maxCoord = *orMaxCoord;
    }
}

void AABB::compose(pbge::AABB *other, pbge::AABB *result) {
    *(result->minCoord) = math3d::Min(*(other->minCoord), *(this->minCoord));
    *(result->maxCoord) = math3d::Max(*(other->maxCoord), *(this->maxCoord));
}

void AABB::update(const math3d::matrix44 &worldTransform) {
    if(coordSpace == AABB::OBJECT_SPACE) {
        // THE TODO
    }
    // else it's already in world coordinate
}

void AABB::distance(const math3d::vector4 & point) {
    math3d::vector4 nearestPoint = math3d::Max(*minCoord, math3d::Min(point, *maxCoord));
    return (point - nearestPoint).size();
}

// Frustum AABB collision procedure
// Implementation based on the code present on http://www.flipcode.com/archives/Frustum_Culling.shtml
bool AABB::frustumCullingTest(BoundingFrustum *bf) {
    return true;
    math3d::vector4 vertices[8];
    bool result = true;
    vertices[0] = *minCoord;
    vertices[1] = math3d::vector4((*maxCoord)[0], (*minCoord)[1], (*minCoord)[2], 1.0f);
    vertices[2] = math3d::vector4((*maxCoord)[0], (*maxCoord)[1], (*minCoord)[2], 1.0f);
    vertices[3] = math3d::vector4((*minCoord)[0], (*maxCoord)[1], (*minCoord)[2], 1.0f);
    vertices[4] = math3d::vector4((*minCoord)[0], (*minCoord)[1], (*maxCoord)[2], 1.0f);
    vertices[5] = math3d::vector4((*maxCoord)[0], (*minCoord)[1], (*maxCoord)[2], 1.0f);
    vertices[6] = math3d::vector4((*minCoord)[0], (*maxCoord)[1], (*maxCoord)[2], 1.0f);
    vertices[7] = *maxCoord;
    std::cout << "testing" << std::endl;
    for(int i = 0; i < 6; i++) {
        int incount = 8;
        math3d::vector4 plane = bf->getPlane(i);
        //std::cout << "plane: " << plane.asString() << std::endl;
        for(int j = 0; j < 8; j++) {
            //std::cout << "point: " << vertices[j].asString() << std::endl;
            if(math3d::dot(plane, vertices[j]) < 0.0f) {
                incount--;
            }
        }
        if(incount == 0) {
            result = false;
            break;
        }
    }
    std::cout << "resultado: " << result << std::endl;
    return result;
}

