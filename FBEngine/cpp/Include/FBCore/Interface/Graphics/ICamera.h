#ifndef _ICamera_H
#define _ICamera_H

#include <FBCore/Interface/Graphics/IFrustum.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Ray3.h>

namespace fb
{
    namespace render
    {

        /**
         * @file ICamera.h
         * @brief A camera for the graphics system.
         */

        /**
         * @class ICamera
         * @brief A camera for the graphics system.
         */
        class ICamera : public IFrustum
        {
        public:
            /**
             * @brief Destructor.
             */
            virtual ~ICamera() override = default;

            /**
             * @brief Gets the position of the camera.
             * @return The position of the camera.
             */
            virtual Vector3F getPosition() const = 0;

            /**
             * @brief Sets the position of the camera.
             * @param position The new position of the camera.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            /**
             * @brief Gets the orientation of the camera.
             * @return The orientation of the camera.
             */
            virtual QuaternionF getOrientation() const = 0;

            /**
             * @brief Sets the orientation of the camera.
             * @param orientation The new orientation of the camera.
             */
            virtual void setOrientation( const QuaternionF &orientation ) = 0;

            /**
             * @brief Gets the direction the camera is facing.
             * @return The direction the camera is facing.
             */
            virtual Vector3F getDirection() const = 0;

            /**
             * @brief Gets the direction from the relative screen coordinates.
             * @param screenPosition The relative screen coordinates.
             * @return The direction from the relative screen coordinates.
             */
            virtual Vector3F getDirection( const Vector2F &screenPosition ) const = 0;

            /**
             * @brief Gets the direction from the relative screen coordinates and returns a world position.
             * @param screenPosition The relative screen coordinates.
             * @param worldPosition The world position.
             * @return The direction from the relative screen coordinates.
             */
            virtual Vector3F getDirection( const Vector2F &screenPosition,
                                           Vector3F &worldPosition ) const = 0;

            /**
             * @brief Gets the up vector of the camera.
             * @return The up vector of the camera.
             */
            virtual Vector3F getUp() const = 0;

            /**
             * @brief Gets the right vector of the camera.
             * @return The right vector of the camera.
             */
            virtual Vector3F getRight() const = 0;

            /**
             * @brief Sets the level-of-detail bias factor for this camera.
             * @param factor The level-of-detail bias factor.
             */
            virtual void setLodBias( f32 factor = 1.0 ) = 0;

            /**
             * @brief Returns the level-of-detail bias factor currently applied to this camera.
             * @return The level-of-detail bias factor currently applied to this camera.
             */
            virtual f32 getLodBias() const = 0;

            /**
             * @brief Gets a world space ray as cast from the camera through a viewport position.
             * @param screenx The x position at which the ray should intersect the viewport, in normalized screen coordinates [0,1].
             * @param screeny The y position at which the ray should intersect the viewport, in normalized screen coordinates [0,1].
             * @return The world space ray as cast from the camera through the viewport position.
             */
            virtual Ray3F getRay( f32 screenx, f32 screeny ) const = 0;

            /**
             * @brief Gets the screen position from the given world position.
             * @param position The world position.
             * @return The screen position from the given world position.
             */
            virtual Vector2F getScreenPosition( const Vector3F &position ) = 0;

            /**
             * @brief Sets the viewing window inside of the viewport.
             * @param Left The left edge of the window.
             * @param Top The top edge of the window.
             * @param Right The right edge of the window.
             * @param Bottom The bottom edge of the window.
             */
            virtual void setWindow( f32 Left, f32 Top, f32 Right, f32 Bottom ) = 0;

            /**
             * @brief Gets the vertical field of view angle in degrees.
             * @return The vertical field of view angle in degrees.
             */
            virtual f32 getFOVy() const = 0;

            /**
             * @brief Sets the vertical field of view angle in degrees.
             * @param fov The vertical field of view angle in degrees.
             */
            virtual void setFOVy( f32 fov ) = 0;

            /**
             * @brief Sets whether the aspect ratio should be automatically calculated by a viewport that owns this frustum whenever the frustum is resized.
             * @param autoratio Whether the aspect ratio should be automatically calculated.
             */
            virtual void setAutoAspectRatio( bool autoratio ) = 0;

            /**
             * @brief Gets whether the aspect ratio should be automatically calculated by a viewport that owns this frustum whenever the frustum is resized.
             * @return Whether the aspect ratio should be automatically calculated.
             */
            virtual bool getAutoAspectRatio() const = 0;

            /**
             * @brief Sets the distance to the near clipping plane of the camera's frustum.
             * @param nearDist The distance to the near clipping plane of the camera's frustum.
             */
            void setNearClipDistance( f32 nearDist ) override = 0;

            /**
             * @brief Gets the distance to the near clipping plane of the camera's frustum.
             * @return The distance to the near clipping plane of the camera's frustum.
             */
            f32 getNearClipDistance() const override = 0;

            /**
             * @brief Sets the distance to the far clipping plane of the camera's frustum.
             * @param farDist The distance to the far clipping plane of the camera's frustum.
             */
            void setFarClipDistance( f32 farDist ) override = 0;

            /**
             * @brief Gets the distance to the far clipping plane of the camera's frustum.
             * @return The distance to the far clipping plane of the camera's frustum.
             */
            f32 getFarClipDistance() const override = 0;

            /**
             * @brief Sets the aspect ratio of the camera's frustum.
             * @param ratio The aspect ratio of the camera's frustum.
             */
            void setAspectRatio( f32 ratio ) override = 0;

            /**
             * @brief Gets the aspect ratio of the camera's frustum.
             * @return The aspect ratio of the camera's frustum.
             */
            f32 getAspectRatio() const override = 0;

            /**
             * @brief Gets the viewport that this camera is associated with.
             * @return The viewport that this camera is associated with.
             */
            virtual SmartPtr<IViewport> getViewport() const = 0;

            /**
             * @brief Sets the viewport that this camera is associated with.
             * @param val The viewport that this camera is associated with.
             */
            virtual void setViewport( SmartPtr<IViewport> val ) = 0;

            /**
             * @brief Gets the view matrix of the camera.
             * @return The view matrix of the camera.
             */
            virtual Matrix4F getViewMatrix() const = 0;

            /**
             * @brief Gets the projection matrix of the camera.
             * @return The projection matrix of the camera.
             */
            virtual Matrix4F getProjectionMatrix() const = 0;

            /**
             * @brief Gets a pointer to the view matrix of the camera in a format suitable for passing to rendering functions.
             * @return A pointer to the view matrix of the camera in a format suitable for passing to rendering functions.
             */
            virtual void *getRenderViewMatrix() const = 0;

            /**
             * @brief Gets a pointer to the projection matrix of the camera in a format suitable for passing to rendering functions.
             * @return A pointer to the projection matrix of the camera in a format suitable for passing to rendering functions.
             */
            virtual void *getRenderProjectionMatrix() const = 0;

            /**
             * @brief Gets the texture that the camera is rendering to.
             * @return The texture that the camera is rendering to.
             */
            virtual SmartPtr<ITexture> getTargetTexture() const = 0;

            /**
             * @brief Sets the texture that the camera should render to.
             * @param targetTexture The texture that the camera should render to.
             */
            virtual void setTargetTexture( SmartPtr<ITexture> targetTexture ) = 0;

            /**
             * @brief Gets the texture that the camera is rendering for editing.
             * @return The texture that the camera is rendering for editing.
             */
            virtual SmartPtr<ITexture> getEditorTexture() const = 0;

            /**
             * @brief Sets the texture that the camera should render for editing.
             * @param editorTexture The texture that the camera should render for editing.
             */
            virtual void setEditorTexture( SmartPtr<ITexture> editorTexture ) = 0;

            /**
             * @brief Sets whether UI objects should be rendered by the camera.
             * @param enabled Whether UI objects should be rendered by the camera.
             */
            virtual void setRenderUI( bool enabled ) = 0;

            /**
             * @brief Gets whether UI objects should be rendered by the camera.
             * @return Whether UI objects should be rendered by the camera.
             */
            virtual bool getRenderUI() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
