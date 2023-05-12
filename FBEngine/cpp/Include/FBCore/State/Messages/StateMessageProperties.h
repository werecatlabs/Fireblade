#ifndef StateMessageProperties_h__
#define StateMessageProperties_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Base/Properties.h"

namespace fb
{
    class StateMessageProperties : public StateMessage
    {
    public:
        StateMessageProperties();
        ~StateMessageProperties() override;

        Properties &getProperties();
        const Properties &getProperties() const;
        void setProperties( const Properties &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Properties m_properties;
    };
}  // end namespace fb

#endif  // StateMessageProperties_h__
