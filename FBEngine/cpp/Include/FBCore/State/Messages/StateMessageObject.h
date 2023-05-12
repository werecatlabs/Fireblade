#ifndef StateMessageObject_h__
#define StateMessageObject_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageObject : public StateMessage
    {
    public:
        StateMessageObject();
        ~StateMessageObject() override;

        SmartPtr<ISharedObject> getObject() const;
        void setObject( SmartPtr<ISharedObject> object );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<ISharedObject> m_object;
    };
}  // end namespace fb

#endif  // StateMessageObject_h__
