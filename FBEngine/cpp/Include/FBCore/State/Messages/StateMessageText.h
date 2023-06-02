#ifndef StateMessageText_h__
#define StateMessageText_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    
    class StateMessageText : public StateMessage
    {
    public:
        StateMessageText() = default;
        ~StateMessageText() override = default;

        String getText() const;
        void setText( const String &value );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_text;
    };

}  // end namespace fb

#endif  // StateMessageText_h__
