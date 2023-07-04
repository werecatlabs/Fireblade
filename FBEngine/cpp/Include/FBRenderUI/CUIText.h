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

            void setVerticalAlignment( u8 alignment );
            u8 getVerticalAlignment() const;

            void setHorizontalAlignment( u8 alignment )
            {
                
            }

            u8 getHorizontalAlignment() const
            {
                return 0;
            }

            void onToggleVisibility() override;
            void onToggleHighlight() override;

            void _getObject( void **ppObject ) const override;

            SmartPtr<render::IOverlayElementText> getOverlayText() const;

            void setOverlayText( SmartPtr<render::IOverlayElementText> overlayText );

            void handleStateChanged( SmartPtr<IState> &state );

            FB_CLASS_REGISTER_DECL;

        private:
            /** Overlay text object. */
            SmartPtr<render::IOverlayElementText> m_overlayText;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
