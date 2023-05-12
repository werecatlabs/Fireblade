#ifndef StateMessageType_h__
#define StateMessageType_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //--------------------------------------------
    class StateMessageType : public StateMessage
    {
    public:
        StateMessageType();
        ~StateMessageType() override;

        u32 getTypeValue() const;
        void setTypeValue( u32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        u32 m_typeValue = 0;
    };
}  // end namespace fb

#endif  // StateMessageType_h__
