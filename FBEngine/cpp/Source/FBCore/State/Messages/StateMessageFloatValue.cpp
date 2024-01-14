#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageFloatValue.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageFloatValue, StateMessage );

    const hash_type StateMessageFloatValue::LEFT_HASH = StringUtil::getHash( "left" );
    const hash_type StateMessageFloatValue::TOP_HASH = StringUtil::getHash( "top" );
    const hash_type StateMessageFloatValue::WIDTH_HASH = StringUtil::getHash( "width" );
    const hash_type StateMessageFloatValue::HEIGHT_HASH = StringUtil::getHash( "height" );

    //--------------------------------------------
    auto StateMessageFloatValue::getValue() const -> f32
    {
        return m_value;
    }

    //--------------------------------------------
    void StateMessageFloatValue::setValue( f32 val )
    {
        m_value = val;
    }
}  // end namespace fb
