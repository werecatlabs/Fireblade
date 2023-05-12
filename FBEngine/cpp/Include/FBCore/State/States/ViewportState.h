#ifndef ViewportState_h__
#define ViewportState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Base/ColourF.h>
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

        Vector2F getPosition() const;
        void setPosition( const Vector2F &position );

        Vector2F getSize() const;
        void setSize( const Vector2F &size );

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

        FB_CLASS_REGISTER_DECL;

    protected:
        atomic_s32 m_zOrder = -1;
        atomic_u32 m_visibilityMask = std::numeric_limits<u32>::max();
        AtomicSmartPtr<render::ICamera> m_camera;
        AtomicValue<ColourF> m_backgroundColour;
        AtomicValue<Vector2F> m_position;
        AtomicValue<Vector2F> m_size;
        atomic_bool m_overlaysEnabled = true;
        atomic_bool m_skiesEnabled = true;
        atomic_bool m_active = true;
    };
}  // end namespace fb

#endif  // ViewportState_h__
