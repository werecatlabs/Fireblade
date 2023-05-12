#ifndef IGUIDialogBox_h__
#define IGUIDialogBox_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIDialogBox
         * @brief Interface for a dialog box, responsible for managing dialog box display in the user interface
         */
        class IUIDialogBox : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIDialogBox() override = default;

            /**
             * @brief Shows the dialog box, does not block; use the listener to handle events
             * @return A boolean indicating whether the operation was successful
             */
            virtual bool show() = 0;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIDialogBox_h__
