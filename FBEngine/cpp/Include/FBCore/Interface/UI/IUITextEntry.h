#ifndef IGUITextEntry_h__
#define IGUITextEntry_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class IUITextEntry : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUITextEntry() override = default;

            virtual void setText( const String &text ) = 0;
            virtual String getText() const = 0;

            virtual void setTextSize( f32 textSize ) = 0;
            virtual f32 getTextSize() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IGUITextEntry_h__
