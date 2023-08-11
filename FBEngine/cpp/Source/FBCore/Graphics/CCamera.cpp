#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CCamera.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CCamera::CCamera()
        {
        }

        CCamera::~CCamera()
        {
        }

        Vector3F CCamera::getPosition() const
        {
            return Vector3F();
        }

        void CCamera::setPosition( const Vector3F &position )
        {
        }

        QuaternionF CCamera::getOrientation() const
        {
            return QuaternionF();
        }

        void CCamera::setOrientation( const QuaternionF &orientation )
        {
        }

        Vector3F CCamera::getDirection() const
        {
            return Vector3F();
        }

        Vector3F CCamera::getUp() const
        {
            return Vector3F();
        }

        Vector3F CCamera::getRight() const
        {
            return Vector3F();
        }

        void CCamera::setLodBias( f32 factor /*= 1.0 */ )
        {
        }

        f32 CCamera::getLodBias() const
        {
            return 0.0f;
        }

        Ray3F CCamera::getRay( f32 screenx, f32 screeny ) const
        {
            return Ray3F();
        }

        Vector2F CCamera::getScreenPosition( const Vector3F &position )
        {
            return Vector2F();
        }

        void CCamera::setWindow( f32 Left, f32 Top, f32 Right, f32 Bottom )
        {
        }

        f32 CCamera::getFOVy() const
        {
            return 0.0f;
        }

        void CCamera::setFOVy( f32 fov )
        {
        }

        void CCamera::setAutoAspectRatio( bool autoratio )
        {
        }

        bool CCamera::getAutoAspectRatio() const
        {
            return false;
        }

        void CCamera::setNearClipDistance( f32 nearDist )
        {
        }

        f32 CCamera::getNearClipDistance() const
        {
            return 0.0f;
        }

        void CCamera::setFarClipDistance( f32 farDist )
        {
        }

        f32 CCamera::getFarClipDistance() const
        {
            return 0.0f;
        }

        void CCamera::setAspectRatio( f32 ratio )
        {
        }

        f32 CCamera::getAspectRatio() const
        {
            return 0.0f;
        }

        SmartPtr<IViewport> CCamera::getViewport() const
        {
            return nullptr;
        }

        void CCamera::setViewport( SmartPtr<IViewport> val )
        {
            
        }

        Matrix4F CCamera::getViewMatrix() const
        {
            return Matrix4F();
        }

        Matrix4F CCamera::getProjectionMatrix() const
        {
            return Matrix4F();
        }

        void *CCamera::getRenderViewMatrix() const
        {
            return nullptr;
        }

        void *CCamera::getRenderProjectionMatrix() const
        {
            return nullptr;
        }

        SmartPtr<ITexture> CCamera::getTargetTexture() const
        {
            return nullptr;
        }

        void CCamera::setTargetTexture( SmartPtr<ITexture> targetTexture )
        {
        }

        SmartPtr<ITexture> CCamera::getEditorTexture() const
        {
            return nullptr;
        }

        void CCamera::setEditorTexture( SmartPtr<ITexture> editorTexture )
        {
        }

        void CCamera::setRenderUI( bool enabled )
        {
        }

        bool CCamera::getRenderUI() const
        {
            return false;
        }

        bool CCamera::isVisible( const AABB3F &bound ) const
        {
            return false;
        }

        bool CCamera::isVisible( const Vector3F &vert ) const
        {
            return false;
        }

        bool CCamera::isVisible( const Sphere3F &bound ) const
        {
            return false;
        }

        Vector3F CCamera::getDirection( const Vector2F &screenPosition,
                                        Vector3F &worldPosition ) const
        {
            return Vector3F();
        }

        Vector3F CCamera::getDirection( const Vector2F &screenPosition ) const
        {
            return Vector3F();
        }
    } // namespace render
}     // namespace fb
