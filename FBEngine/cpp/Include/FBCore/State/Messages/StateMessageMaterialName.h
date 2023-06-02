#ifndef StateMessageMaterialName_h__
#define StateMessageMaterialName_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Core/StringTypes.h"

namespace fb
{
    //---------------------------------------------
    class StateMessageMaterialName : public StateMessage
    {
    public:
        StateMessageMaterialName() = default;
        ~StateMessageMaterialName() override = default;

        String getMaterialName() const;
        void setMaterialName( const String &val );

        u32 getIndex() const;
        void setIndex( u32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_value;
        u32 m_index = 0;
    };
}  // end namespace fb

#endif  // StateMessageMaterialName_h__
