
#ifndef PBGE_GFX_CAMERA_H_
#define PBGE_GFX_CAMERA_H_

#include <string>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/scoped_array.hpp>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/collision/BoundingVolumes.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/RenderTarget.h"

namespace pbge {
    class CameraNode;

    /** The Frustum is responsible for calculating the projection matrix
    */
    class PBGE_EXPORT Frustum {
    public:
        Frustum():projectionMatrix(new math3d::matrix44) {
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
        
        const math3d::matrix44 getProjectionMatrix() const {
            return *projectionMatrix;
        }
    private:
        boost::scoped_ptr<math3d::matrix44> projectionMatrix;
    };


    /** The Pandora's Box default camera.

        This camera class specifies a camera transformation in object space
    */
    class PBGE_EXPORT Camera {
    public:
        Frustum frustum;
        
        /** The default camera constructor.
            
            This constructor initializes the camera looking at (0,0,-1) and 
            with an up vector pointing to (0,1,0)
        */
        Camera ();

        /** Constructor with a specified camera transformation.
        */
        Camera (const math3d::vector4 & up, const math3d::vector4 & front);
        
        /** Releases all the camera resources.
        */
        ~Camera();

        /** Calculates a new view transformation.

            This method calculates a view transformation that represents a camera
            looking to the direction pointed by front and with up vector given by up

            @param up the camera up direction
            @param front the direction the camera is pointing
        */
        void lookAt (const math3d::vector4 & up, const math3d::vector4 & front);

        /** Sets new viewport to be used by this camera

            The viewport is currently not used
        */
        void setViewport (Viewport * port) {
            viewport = port;
        }
        
        /** Loads the view and projection transformation into the GraphicAPI.

            @param gfx The api that should be used
        */
        virtual void setCamera (GraphicAPI * gfx);

        /** Clear the viewport specific changes.
        */
        virtual void unsetCamera (GraphicAPI * gfx);
        
        /** Appends the camera to the node.
        */
        void setParent(CameraNode * node) { parent = node; }

        /** Returns the camera parent on the scene graph.
        */
        CameraNode * getParent() { return parent; }

        /** Returns the current view transformation.
        */
        const math3d::matrix44 & getViewTransform() {
            return *viewTransform;
        }

        /** Returns the bounding frustum that is used for the frustum culling.
        */
        BoundingFrustum * getBoundingFrustum() {
            return &boundingFrustum;
        }
    protected:
        CameraNode * parent;
        Viewport * viewport;
        BoundingFrustum boundingFrustum;
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
