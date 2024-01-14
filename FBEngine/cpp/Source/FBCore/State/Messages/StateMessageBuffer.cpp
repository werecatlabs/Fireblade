#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageBuffer.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageBuffer, StateMessage );

    StateMessageBuffer::StateMessageBuffer()
    {
    }

    StateMessageBuffer::~StateMessageBuffer()
    {
        FB_SAFE_DELETE( m_buffer );
    }

    auto StateMessageBuffer::getBuffer() const -> u8 *
    {
        return m_buffer;
    }

    void StateMessageBuffer::setBuffer( u8 *val )
    {
        m_buffer = val;
    }
}  // namespace fb
