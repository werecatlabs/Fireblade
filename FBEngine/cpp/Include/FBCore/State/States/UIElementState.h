#ifndef UIElementState_h__
#define UIElementState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/ColourF.h>

namespace fb
{
    class UIElementState : public BaseState
    {
    public:
        UIElementState();

        ~UIElementState() override;

        Vector2<real_Num> getPosition() const;

        void setPosition( const Vector2<real_Num> &position );

        Vector2<real_Num> getSize() const;

        void setSize( const Vector2<real_Num> &size );

        void setVisible( bool visible );

        bool isVisible() const;

        bool getElementVisible() const;

        void setElementVisible( bool elementVisible );

        SmartPtr<render::IMaterial> getMaterial() const;

        void setMaterial( SmartPtr<render::IMaterial> material );

        SmartPtr<render::ITexture> getTexture() const;
        void setTexture( SmartPtr<render::ITexture> texture );

        String getCaption() const;
        void setCaption( const String &caption );

        ColourF getColour() const;

        void setColour( const ColourF &colour );

        u32 getTextSize() const;
        void setTextSize( u32 textSize );

        u32 getZOrder() const;

        void setZOrder( u32 zOrder );

        bool getHandleInputEvents() const;

        void setHandleInputEvents( bool handleInputEvents );

        SmartPtr<IState> clone() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        AtomicSmartPtr<render::IMaterial> m_material;
        AtomicSmartPtr<render::ITexture> m_texture;

        ColourF m_colour = ColourF::White;

        u32 m_zOrder = 0;

        u8 m_metricsMode = 0;
        u8 m_gha = 0;
        u8 m_gva = 0;

        Vector2<real_Num> m_position = Vector2<real_Num>::zero();
        Vector2<real_Num> m_size = Vector2<real_Num>::unit();
        String m_name;
        String m_caption;

        u32 m_textSize = 12;

        bool m_visible = true;
        bool m_handleInputEvents = true;

        bool m_elementVisible = true;
    };
}  // end namespace fb

#endif  // UIElementState_h__
