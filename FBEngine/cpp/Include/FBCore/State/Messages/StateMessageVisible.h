#ifndef StateMessageVisible_h__
#define StateMessageVisible_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageVisible : public StateMessage
    {
    public:
        StateMessageVisible() = default;
        ~StateMessageVisible() override = default;

        bool isVisible() const;
        void setVisible( bool val );

        bool getCascade() const;
        void setCascade( bool val );

        FB_CLASS_REGISTER_DECL;

    protected:
        bool m_isVisible = true;
        bool m_cascade = true;
    };
}  // end namespace fb

#endif  // StateMessageVisible_h__
