#include <FBCore/FBCorePCH.h>
#include "FBCore/State/States/TextureState.h"
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TextureState, BaseState );

    TextureState::TextureState() = default;
    TextureState::~TextureState() = default;

    auto TextureState::clone() -> SmartPtr<IState>
    {
        return nullptr;
    }

    auto TextureState::getSize() const -> Vector2I
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_size;
    }

    void TextureState::setSize( const Vector2I &size )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        if( m_size != size )
        {
            m_size = size;
            setDirty( true );
        }
    }

}  // namespace fb
