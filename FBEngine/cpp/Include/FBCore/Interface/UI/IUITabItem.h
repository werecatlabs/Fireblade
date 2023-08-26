#ifndef IUITabItem_h__
#define IUITabItem_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUITabItem : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUITabItem() override = default;

            virtual String getLabel() const = 0;

            virtual void setLabel( const String &label ) = 0;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUITabItem_h__
