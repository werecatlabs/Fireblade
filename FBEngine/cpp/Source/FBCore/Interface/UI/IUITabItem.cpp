#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUITabItem.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUITabItem, IUIElement );

        String IUITabItem::getLabel() const
        {
            return m_label;
        }

        void IUITabItem::setLabel( const String &label )
        {
            m_label = label;
        }

    }  // end namespace ui
}  // end namespace fb
