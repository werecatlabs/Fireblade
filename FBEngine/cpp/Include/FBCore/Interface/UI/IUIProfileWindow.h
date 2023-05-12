#ifndef IUIProfileWindow_h__
#define IUIProfileWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIProfileWindow
         * @brief Interface for a UI Profile Window, extending IUIElement and providing functionality for managing a single profile
         */
        class IUIProfileWindow : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIProfileWindow() override = default;

            /**
             * @brief Gets the IProfile associated with the UI Profile Window
             * @return A shared pointer to the IProfile object
             */
            virtual SmartPtr<IProfile> getProfile() const = 0;

            /**
             * @brief Sets the IProfile associated with the UI Profile Window
             * @param profile A shared pointer to the IProfile object to be associated with the window
             */
            virtual void setProfile( SmartPtr<IProfile> profile ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIProfileWindow_h__
