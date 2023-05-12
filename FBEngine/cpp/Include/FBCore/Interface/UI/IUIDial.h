#ifndef IGUIDial_h__
#define IGUIDial_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUIDial : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUIDial() override = default;

            virtual void setNeedlePosition( f32 position ) = 0;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIDial_h__
