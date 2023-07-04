#ifndef _CGUIItemTemplate_H
#define _CGUIItemTemplate_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include "FBCore/Core/Properties.h"
#include <FBRenderUI/CUIElement.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIItemTemplate : public CUIElement<IUIElement>
        {
        public:
            CUIItemTemplate();
            ~CUIItemTemplate() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            const Properties &getPropertyGroup() const;
            Properties &getPropertyGroup();

        private:
            Properties m_propertyGroup;
        };
    } // end namespace ui
}     // end namespace fb

#endif
