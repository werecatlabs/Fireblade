#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgre/Wrapper/CDebugText.h>
#include <FBCore/FBCore.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, CDebugText, IDebugText );

    CDebugText::CDebugText() = default;

    CDebugText::~CDebugText() = default;

    void CDebugText::load( SmartPtr<ISharedObject> data )
    {
    }

    void CDebugText::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto CDebugText::getText() const -> String
    {
        return m_text;
    }

    void CDebugText::setText( const String &text )
    {
        m_text = text;
    }

    auto CDebugText::getTextElement() const -> SmartPtr<IOverlayElementText>
    {
        return m_textElement;
    }

    void CDebugText::setTextElement( SmartPtr<IOverlayElementText> textElement )
    {
        m_textElement = textElement;
    }
}  // namespace fb::render
