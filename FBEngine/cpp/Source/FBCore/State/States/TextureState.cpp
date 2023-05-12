#include <FBCore/FBCorePCH.h>
#include "FBCore/State/States/TextureState.h"
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TextureState, BaseState );

    SmartPtr<IState> TextureState::clone()
    {
        return nullptr;
    }

    Vector2I TextureState::getSize() const
    {
        return m_size;
    }

    void TextureState::setSize( const Vector2I &size )
    {
        if( m_size != size )
        {
            m_size = size;
            setDirty( true );
        }
    }

}  // namespace fb
