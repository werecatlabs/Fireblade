#ifndef CGUIVector_h__
#define CGUIVector_h__

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIVector3.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        class CUIVector : public CUIElement<IUIVector3>
        {
        public:
            CUIVector();
            ~CUIVector() override;

            void initialise( SmartPtr<IUIElement> &parent ) override;
            void initialise( SmartPtr<IUIElement> &parent, const Properties &properties ) override;
            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            String getFileName() const;
            void setFileName( const String &val );

            SmartPtr<render::IMaterial> getMaterial() const;
            void setMaterial( SmartPtr<render::IMaterial> val );

        protected:
            String m_fileName;
            SmartPtr<render::IOverlayElementVector> m_container;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // CGUIVector_h__
