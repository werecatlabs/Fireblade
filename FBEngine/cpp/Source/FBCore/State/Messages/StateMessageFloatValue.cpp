#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageFloatValue.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageFloatValue, StateMessage );

    const hash_type StateMessageFloatValue::LEFT_HASH = StringUtil::getHash( "left" );
    const hash_type StateMessageFloatValue::TOP_HASH = StringUtil::getHash( "top" );
    const hash_type StateMessageFloatValue::WIDTH_HASH = StringUtil::getHash( "width" );
    const hash_type StateMessageFloatValue::HEIGHT_HASH = StringUtil::getHash( "height" );

    //--------------------------------------------
    f32 StateMessageFloatValue::getValue() const
    {
        return m_value;
    }

    //--------------------------------------------
    void StateMessageFloatValue::setValue( f32 val )
    {
        m_value = val;
    }
}  // end namespace fb
