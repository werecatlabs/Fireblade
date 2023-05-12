#ifndef _CGUICONTAINER_H
#define _CGUICONTAINER_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>
#include <FBCore/Interface/UI/IUIContainer.h>

namespace fb
{
    namespace ui
    {
        
        class CUIContainer : public CUIElement<IUIContainer>
        {
        public:
            CUIContainer();
            ~CUIContainer() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            void setPosition( const Vector2F &position ) override;

            SmartPtr<render::IOverlayElementContainer> getOverlayContainer() const;
            void setOverlayContainer( SmartPtr<render::IOverlayElementContainer> val );

            void onAddChild( IUIElement *child ) override;
            void onRemoveChild( IUIElement *child ) override;

            void _getObject( void **ppObject ) const override;

        private:
            void onToggleVisibility() override;

            SmartPtr<render::IOverlayElementContainer> m_overlayContainer;
        };
    } // end namespace ui
}     // end namespace fb

#endif
