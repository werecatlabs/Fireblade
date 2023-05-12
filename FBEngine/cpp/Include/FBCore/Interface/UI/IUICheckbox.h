#ifndef IUICheckbox_h__
#define IUICheckbox_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUICheckbox : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUICheckbox() override = default;

            virtual void setValue( bool value ) = 0;
            virtual bool getValue() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IGUICheckbox_h__
