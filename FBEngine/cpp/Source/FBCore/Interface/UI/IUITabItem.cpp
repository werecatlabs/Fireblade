#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUITabItem.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, IUITabItem, IUIElement );

    auto IUITabItem::getLabel() const -> String
    {
        return m_label;
    }

    void IUITabItem::setLabel( const String &label )
    {
        m_label = label;
    }

}  // namespace fb::ui
