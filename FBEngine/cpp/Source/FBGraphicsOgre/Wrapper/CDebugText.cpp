#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDebugText.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, CDebugText, CSharedObject<IDebugText> );

        CDebugText::CDebugText()
        {
        }

        CDebugText::~CDebugText()
        {
        }

        void CDebugText::load( SmartPtr<ISharedObject> data )
        {
        }

        void CDebugText::unload( SmartPtr<ISharedObject> data )
        {
        }

        String CDebugText::getText() const
        {
            return m_text;
        }

        void CDebugText::setText( const String &text )
        {
            m_text = text;
        }

        SmartPtr<IOverlayElementText> CDebugText::getTextElement() const
        {
            return m_textElement;
        }

        void CDebugText::setTextElement( SmartPtr<IOverlayElementText> textElement )
        {
            m_textElement = textElement;
        }

    }  // namespace render
}  // namespace fb
