#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUICheckBox.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{

    CUICheckBox::CUICheckBox() = default;

    CUICheckBox::~CUICheckBox() = default;

    void CUICheckBox::setValue( bool value )
    {
    }

    auto CUICheckBox::getValue() const -> bool
    {
        return false;
    }

}  // namespace fb::ui
