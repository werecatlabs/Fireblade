#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/OverlayTextState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, OverlayTextState, OverlayElementState );

    OverlayTextState::OverlayTextState() = default;
    OverlayTextState::~OverlayTextState() = default;

    auto OverlayTextState::getText() const -> String
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_text;
    }

    void OverlayTextState::setText( const String &text )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_text != text )
        {
            m_text = text;
            setDirty( true );
        }
    }

    auto OverlayTextState::getAlignment() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_alignment;
    }

    void OverlayTextState::setAlignment( u32 alignment )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_alignment = alignment;
        setDirty( true );
    }

}  // end namespace fb
