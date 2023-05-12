#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageText.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageText, StateMessage );

    String StateMessageText::getText() const
    {
        return m_text;
    }

    void StateMessageText::setText( const String &value )
    {
        m_text = value;
    }
}  // namespace fb
