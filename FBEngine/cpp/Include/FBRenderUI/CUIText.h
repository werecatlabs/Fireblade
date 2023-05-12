#ifndef _CGUITEXT_H
#define _CGUITEXT_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIText.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {

        class CUIText : public CUIElement<IUIText>
        {
        public:
            CUIText();
            ~CUIText() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setPosition( const Vector2F &position ) override;
            void setSize( const Vector2F &size );

            void setText( const String &text ) override;
            String getText() const override;

            void setTextSize( f32 textSize ) override;
            f32 getTextSize() const override;

            s32 setProperty( hash32 hash, const String &value ) override;
            s32 getProperty( hash32 hash, String &value ) const override;

            void _getObject( void **ppObject ) const override;

            //
            // Callbacks
            //
            void onToggleVisibility() override;
            void onToggleHighlight() override;

            SmartPtr<render::IOverlayElementText> getOverlayText() const;

            void setOverlayText( SmartPtr<render::IOverlayElementText> overlayText );

            void handleStateChanged( SmartPtr<IState> &state );

            FB_CLASS_REGISTER_DECL;

        private:
            SmartPtr<render::IOverlayElementText> m_overlayText;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
