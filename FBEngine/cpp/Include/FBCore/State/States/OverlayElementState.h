#ifndef FB_OverlayElementState_H
#define FB_OverlayElementState_H

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Core/ColourF.h>

namespace fb
{
    class OverlayElementState : public BaseState
    {
    public:
        OverlayElementState() = default;
        ~OverlayElementState() override = default;

        Vector2F getPosition() const;

        void setPosition( const Vector2F &position );

        Vector2F getSize() const;

        void setSize( const Vector2F &size );

        String getName() const;

        void setName( const String &name );

        void setMetricsMode( [[maybe_unused]] u8 metricsMode );

        u8 getMetricsMode() const;

        void setHorizontalAlignment( [[maybe_unused]] u8 gha );

        u8 getHorizontalAlignment() const;

        void setVerticalAlignment( [[maybe_unused]] u8 gva );

        u8 getVerticalAlignment() const;

        SmartPtr<render::IMaterial> getMaterial() const;

        void setMaterial( SmartPtr<render::IMaterial> material );

        void setCaption( const String &text );

        String getCaption() const;

        void setVisible( bool visible );

        bool isVisible() const;

        u32 getZOrder() const;

        void setZOrder( u32 zOrder );

        void setColour( const ColourF &colour );

        ColourF getColour() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<render::IMaterial> m_material;

        ColourF m_colour = ColourF::White;

        u32 m_zOrder = 0;

        atomic_u8 m_metricsMode = 0;
        atomic_u8 m_gha = 0;
        atomic_u8 m_gva = 0;

        Vector2F m_position = Vector2F::zero();
        Vector2F m_size = Vector2F::unit();
        String m_name;
        String m_caption;

        atomic_bool m_visible = true;
    };
} // end namespace fb

#endif  // FB_OverlayElementState_H
