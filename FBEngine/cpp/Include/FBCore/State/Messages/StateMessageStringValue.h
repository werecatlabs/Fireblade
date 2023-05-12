#ifndef StateMessageStringValue_h__
#define StateMessageStringValue_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Base/StringTypes.h"

namespace fb
{
    //---------------------------------------------
    class StateMessageStringValue : public StateMessage
    {
    public:
        StateMessageStringValue();
        ~StateMessageStringValue() override;

        String getValue() const;
        void setValue( const String &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_value;
    };
}  // end namespace fb

#endif  // StateMessageStringValue_h__
