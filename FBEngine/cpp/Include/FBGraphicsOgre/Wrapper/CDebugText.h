#ifndef CDebugText_h__
#define CDebugText_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDebugText.h>
#include <FBCore/Memory/SharedObject.h>
#include <OgreMaterial.h>

namespace fb
{
    namespace render
    {
        class CDebugText : public SharedObject<IDebugText>
        {
        public:
            CDebugText();
            ~CDebugText() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            String getText() const override;
            void setText( const String &text ) override;

            SmartPtr<IOverlayElementText> getTextElement() const override;
            void setTextElement( SmartPtr<IOverlayElementText> textElement ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            hash32 m_id = 0;
            String m_text;
            SmartPtr<IOverlayElementText> m_textElement;
        };
    }  // namespace render
}  // namespace fb

#endif  // CDebugText_h__
