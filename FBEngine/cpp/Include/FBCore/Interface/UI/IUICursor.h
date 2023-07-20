#ifndef _IUICursor_H
#define _IUICursor_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        /**
         * @class IUICursor
         * @brief Interface for a cursor UI element that displays a cursor in the user interface.
         */
        class IUICursor : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUICursor() override = default;

            /** */
            virtual void setMaterialName( const String &materialName ) = 0;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
