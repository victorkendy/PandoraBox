
#ifndef PBGE_GFX_CAMERA_H_
#define PBGE_GFX_CAMERA_H_

#include <string>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/scoped_array.hpp>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/RenderTarget.h"

namespace pbge {
    class CameraNode;

    class PBGE_EXPORT Frustum {
    public:
        Frustum():frustumPlanes(new math3d::vector4[6]),frustumPoints(new math3d::vector4[8]),projectionMatrix(new math3d::matrix44) {
        }

        ~Frustum();

        // Sets the frustum for perspective viewing
        void setPerspective (const float & fovy, const float & aspect,
                             const float & near, const float & far);
        // Sets the orthographic projection
        void setOrtho (const float & left, const float & right,
                       const float & bottom, const float & top,
                       const float & near, const float & far);
        // Use this only if you know what you are doing
        void setFrustum (const float & left, const float & right,
                         const float & bottom, const float & top,
                         const float & near, const float & far);
        // If you are really good send me a ready to use matrix
        void setProjectionMatrix (const math3d::matrix44 & newProjection);
        
        // Send the projection matrix to the current active opengl matrix
        void loadProjection (GraphicAPI * ogl) const;
    private:
        void updateFrustumPlanes();
        void updatePerspectivePoints();
        void updateOrthoPoints();
        void updateFrustumPoints();
        boost::scoped_array<math3d::vector4> frustumPlanes;
        boost::scoped_array<math3d::vector4> frustumPoints;
        boost::scoped_ptr<math3d::matrix44> projectionMatrix;
    };


    class PBGE_EXPORT Camera {
    public:
        Frustum frustum;
        
        // The default camera points to (0,0,-1) and has the up vector (0,1,0) 
        Camera ();
        // Sets the camera with the required view transformation
        Camera (const math3d::vector4 & up, const math3d::vector4 & front);
        
        ~Camera();

        // Changes the camera view transformation
        void lookAt (const math3d::vector4 & up, const math3d::vector4 & front);

        // Changes where de renderer will render the scene
        void setViewport (Viewport * port) {
            viewport = port;
        }
        
        // Load the view transformation to opengl and sets the render target
        virtual void setCamera (GraphicAPI * ogl);
        // unset the render target
        virtual void unsetCamera (GraphicAPI * ogl);
        
        void setParent(CameraNode * node) { parent = node; }

        CameraNode * getParent() { return parent; }

        const math3d::matrix44 & getViewTransform() {
            return *viewTransform;
        }
    protected:
        CameraNode * parent;
        Viewport * viewport;
        boost::scoped_ptr<math3d::vector4> upVector;
        boost::scoped_ptr<math3d::vector4> frontVector;
        boost::scoped_ptr<math3d::vector4> sideVector;
        boost::scoped_ptr<math3d::matrix44> cameraTransformation;
        boost::scoped_ptr<math3d::matrix44> viewTransform;
        void setCameraTransformation ();
        int is_valid;
    };
}
#endif
