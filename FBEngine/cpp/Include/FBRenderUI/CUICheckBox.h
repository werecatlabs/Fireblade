#ifndef CUICheckBox_h__
#define CUICheckBox_h__

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUICheckbox.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        class CUICheckBox : public CUIElement<IUICheckbox>
        {
        public:
            CUICheckBox();
            ~CUICheckBox() override;

            void setValue( bool value ) override;

            bool getValue() const override;
        };
    } // namespace ui
}     // namespace fb

#endif  // CGUICheckBox_h__
