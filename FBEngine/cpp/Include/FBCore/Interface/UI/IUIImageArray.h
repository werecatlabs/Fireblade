#ifndef IGUIImageArray_h__
#define IGUIImageArray_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        //---------------------------------------------
        class IUIImageArray : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIImageArray() override = default;

            /** */
            virtual SmartPtr<IUIImage> getImage( u32 index ) = 0;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIImageArray_h__
