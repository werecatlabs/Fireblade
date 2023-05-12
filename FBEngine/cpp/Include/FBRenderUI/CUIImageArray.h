#ifndef CGUIImageArray_h__
#define CGUIImageArray_h__

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIImageArray.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIImageArray : public CUIElement<IUIImageArray>
        {
        public:
            CUIImageArray();
            ~CUIImageArray() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            SmartPtr<IUIImage> getImage( u32 index ) override;

        private:
            void onAddChild( IUIElement *child ) override;
            void onRemoveChild( IUIElement *child ) override;

            Array<SmartPtr<IUIImage>> m_images;

            SmartPtr<render::IOverlayElementContainer> m_container;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // CGUIImageArray_h__
