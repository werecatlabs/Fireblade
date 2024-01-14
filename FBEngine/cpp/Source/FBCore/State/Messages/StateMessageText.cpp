#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageText.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageText, StateMessage );

    auto StateMessageText::getText() const -> String
    {
        return m_text;
    }

    void StateMessageText::setText( const String &value )
    {
        m_text = value;
    }
}  // namespace fb
