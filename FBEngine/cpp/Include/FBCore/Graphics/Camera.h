#ifndef CCamera_h__
#define CCamera_h__

#include <FBCore/Graphics/GraphicsObject.h>
#include <FBCore/Interface/Graphics/ICamera.h>

namespace fb
{
    namespace render
    {
        class Camera : public GraphicsObject<ICamera>
        {
        public:
            Camera();
            ~Camera() override;

            Vector3<real_Num> getPosition() const override;

            void setPosition( const Vector3<real_Num> &position ) override;

            Quaternion<real_Num> getOrientation() const override;

            void setOrientation( const Quaternion<real_Num> &orientation ) override;

            Vector3<real_Num> getDirection() const override;

            Vector3<real_Num> getDirection( const Vector2<real_Num> &screenPosition ) const override;

            Vector3<real_Num> getDirection( const Vector2<real_Num> &screenPosition,
                                   Vector3<real_Num> &worldPosition ) const override;

            Vector3<real_Num> getUp() const override;

            Vector3<real_Num> getRight() const override;

            void setLodBias( f32 factor = 1.0 ) override;

            f32 getLodBias() const override;

            Ray3<real_Num> getRay( f32 screenx, f32 screeny ) const override;

            Vector2<real_Num> getScreenPosition( const Vector3<real_Num> &position ) override;

            void setWindow( f32 Left, f32 Top, f32 Right, f32 Bottom ) override;

            f32 getFOVy() const override;

            void setFOVy( f32 fov ) override;

            void setAutoAspectRatio( bool autoratio ) override;

            bool getAutoAspectRatio() const override;

            void setNearClipDistance( f32 nearDist ) override;

            f32 getNearClipDistance() const override;

            void setFarClipDistance( f32 farDist ) override;

            f32 getFarClipDistance() const override;

            void setAspectRatio( f32 ratio ) override;

            f32 getAspectRatio() const override;

            SmartPtr<IViewport> getViewport() const override;

            void setViewport( SmartPtr<IViewport> val ) override;

            Matrix4F getViewMatrix() const override;

            Matrix4F getProjectionMatrix() const override;

            void *getRenderViewMatrix() const override;

            void *getRenderProjectionMatrix() const override;

            SmartPtr<ITexture> getTargetTexture() const override;

            void setTargetTexture( SmartPtr<ITexture> targetTexture ) override;

            SmartPtr<ITexture> getEditorTexture() const override;

            void setEditorTexture( SmartPtr<ITexture> editorTexture ) override;

            void setRenderUI( bool enabled ) override;

            bool getRenderUI() const override;

            bool isVisible( const AABB3F &bound ) const override;

            bool isVisible( const Sphere3F &bound ) const override;

            bool isVisible( const Vector3<real_Num> &vert ) const override;
        };
    } // namespace render
}     // namespace fb

#endif  // CCamera_h__
