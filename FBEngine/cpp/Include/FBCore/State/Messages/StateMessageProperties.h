#ifndef StateMessageProperties_h__
#define StateMessageProperties_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageProperties : public StateMessage
    {
    public:
        StateMessageProperties();
        ~StateMessageProperties() override;

        SmartPtr<Properties> getProperties() const override;
        void setProperties( SmartPtr<Properties> properties ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<Properties> m_properties;
    };
} // end namespace fb

#endif  // StateMessageProperties_h__
