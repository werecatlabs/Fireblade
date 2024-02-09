#ifndef ViewportState_h__
#define ViewportState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class ViewportState : public BaseState
    {
    public:
        ViewportState();
        ~ViewportState() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void setBackgroundColour( const ColourF &colour );
        ColourF getBackgroundColour() const;

        Vector2<real_Num> getPosition() const;
        void setPosition( const Vector2<real_Num> &position );

        Vector2<real_Num> getSize() const;
        void setSize( const Vector2<real_Num> &size );

        bool getOverlaysEnabled() const;

        void setOverlaysEnabled( bool overlaysEnabled );

        SmartPtr<render::ICamera> getCamera() const;
        void setCamera( SmartPtr<render::ICamera> camera );

        void setVisibilityMask( u32 mask );
        u32 getVisibilityMask() const;

        void setSkiesEnabled( bool enabled );

        bool getSkiesEnabled() const;

        s32 getZOrder() const;

        void setZOrder( s32 zorder );

        bool isActive() const;

        void setActive( bool active );

        u32 getPriority() const;

        void setPriority( u32 priority );

        void setAutoUpdated( bool autoUpdated );

        bool isAutoUpdated() const;

        SmartPtr<render::ITexture> getTexture() const;

        void setTexture( SmartPtr<render::ITexture> texture );

        SmartPtr<render::ITexture> getBackgroundTexture() const;

        void setBackgroundTexture( SmartPtr<render::ITexture> backgroundTexture );

        bool getShadowsEnabled() const;

        void setShadowsEnabled( bool shadowsEnabled );

        bool getEnableUI() const;
        void setEnableUI( bool enableUI );

        void setMaterialScheme( const String &schemeName );
        String getMaterialScheme() const;

        bool getClear() const;

        void setClear( bool clear );

        u32 getBuffers() const;

        void setBuffers( u32 buffers );

        bool getEnableSceneRender() const;

        void setEnableSceneRender( bool enableSceneRender );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<render::ITexture> m_texture;
        SmartPtr<render::ITexture> m_backgroundTexture;
        atomic_s32 m_zOrder = -1;
        atomic_u32 m_visibilityMask = std::numeric_limits<u32>::max();
        u32 m_priority = 0;

        AtomicSmartPtr<render::ICamera> m_camera;
        AtomicValue<ColourF> m_backgroundColour;
        AtomicValue<Vector2<real_Num>> m_position;
        AtomicValue<Vector2<real_Num>> m_size;
        atomic_bool m_overlaysEnabled = true;
        atomic_bool m_skiesEnabled = true;
        atomic_bool m_active = true;
        bool m_shadowsEnabled = true;
        bool m_autoUpdated = true;
        bool m_enableUI = false;
        bool m_clear = true;
        bool m_enableSceneRender = true;
        u32 m_buffers = 0;

        String m_materialScheme;
    };
}  // end namespace fb

#endif  // ViewportState_h__
