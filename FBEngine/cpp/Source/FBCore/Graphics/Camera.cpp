#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Camera.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Graphics/IViewport.h>

namespace fb::render
{
    Camera::Camera() = default;

    Camera::~Camera() = default;

    auto Camera::getPosition() const -> Vector3F
    {
        return {};
    }

    void Camera::setPosition( const Vector3F &position )
    {
    }

    auto Camera::getOrientation() const -> QuaternionF
    {
        return {};
    }

    void Camera::setOrientation( const QuaternionF &orientation )
    {
    }

    auto Camera::getDirection() const -> Vector3F
    {
        return {};
    }

    auto Camera::getUp() const -> Vector3F
    {
        return {};
    }

    auto Camera::getRight() const -> Vector3F
    {
        return {};
    }

    void Camera::setLodBias( f32 factor /*= 1.0 */ )
    {
    }

    auto Camera::getLodBias() const -> f32
    {
        return 0.0f;
    }

    auto Camera::getRay( f32 screenx, f32 screeny ) const -> Ray3F
    {
        return {};
    }

    auto Camera::getScreenPosition( const Vector3F &position ) -> Vector2F
    {
        return {};
    }

    void Camera::setWindow( f32 Left, f32 Top, f32 Right, f32 Bottom )
    {
    }

    auto Camera::getFOVy() const -> f32
    {
        return 0.0f;
    }

    void Camera::setFOVy( f32 fov )
    {
    }

    void Camera::setAutoAspectRatio( bool autoratio )
    {
    }

    auto Camera::getAutoAspectRatio() const -> bool
    {
        return false;
    }

    void Camera::setNearClipDistance( f32 nearDist )
    {
    }

    auto Camera::getNearClipDistance() const -> f32
    {
        return 0.0f;
    }

    void Camera::setFarClipDistance( f32 farDist )
    {
    }

    auto Camera::getFarClipDistance() const -> f32
    {
        return 0.0f;
    }

    void Camera::setAspectRatio( f32 ratio )
    {
    }

    auto Camera::getAspectRatio() const -> f32
    {
        return 0.0f;
    }

    auto Camera::getViewport() const -> SmartPtr<IViewport>
    {
        return nullptr;
    }

    void Camera::setViewport( SmartPtr<IViewport> val )
    {
    }

    auto Camera::getViewMatrix() const -> Matrix4F
    {
        return {};
    }

    auto Camera::getProjectionMatrix() const -> Matrix4F
    {
        return {};
    }

    auto Camera::getRenderViewMatrix() const -> void *
    {
        return nullptr;
    }

    auto Camera::getRenderProjectionMatrix() const -> void *
    {
        return nullptr;
    }

    auto Camera::getTargetTexture() const -> SmartPtr<ITexture>
    {
        return nullptr;
    }

    void Camera::setTargetTexture( SmartPtr<ITexture> targetTexture )
    {
    }

    auto Camera::getEditorTexture() const -> SmartPtr<ITexture>
    {
        return nullptr;
    }

    void Camera::setEditorTexture( SmartPtr<ITexture> editorTexture )
    {
    }

    void Camera::setRenderUI( bool enabled )
    {
    }

    auto Camera::getRenderUI() const -> bool
    {
        return false;
    }

    auto Camera::isVisible( const AABB3F &bound ) const -> bool
    {
        return false;
    }

    auto Camera::isVisible( const Vector3F &vert ) const -> bool
    {
        return false;
    }

    auto Camera::isVisible( const Sphere3F &bound ) const -> bool
    {
        return false;
    }

    auto Camera::getDirection( const Vector2F &screenPosition, Vector3F &worldPosition ) const
        -> Vector3F
    {
        return {};
    }

    auto Camera::getDirection( const Vector2F &screenPosition ) const -> Vector3F
    {
        return {};
    }
}  // namespace fb::render
