#ifndef _CGUICursor_H
#define _CGUICursor_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUICursor.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUICursor : public CUIElement<IUICursor>
        {
        public:
            CUICursor();
            ~CUICursor() override;

            void initialise();

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void setMaterialName( const String &materialName ) override;

            void setSize( const Vector2F &size ) override;

        private:
            void onToggleVisibility() override;

            SmartPtr<render::IOverlayElementContainer> m_container;
        };
    } // end namespace ui
}     // end namespace fb

#endif
