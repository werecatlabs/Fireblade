#ifndef StateMessageDirty_h__
#define StateMessageDirty_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageDirty : public StateMessage
    {
    public:
        StateMessageDirty() = default;
        ~StateMessageDirty() override = default;

        bool isDirty() const;
        void setDirty( bool dirty );

        FB_CLASS_REGISTER_DECL;

    protected:
        bool m_isDirty = true;
    };
}  // end namespace fb

#endif  // StateMessageDirty_h__
