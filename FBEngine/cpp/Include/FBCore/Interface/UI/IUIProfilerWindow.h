#ifndef IUIProfilerWindow_h__
#define IUIProfilerWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIProfilerWindow
         * @brief Interface for a UI Profiler Window, extending IUIElement and providing functionality for managing multiple profile windows
         */
        class IUIProfilerWindow : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIProfilerWindow() override = default;

            /**
             * @brief Adds a new profile window
             * @return A shared pointer to the newly created IUIProfileWindow object
             */
            virtual SmartPtr<IUIProfileWindow> addProfile() = 0;

            /**
             * @brief Removes a profile window
             * @param profile A shared pointer to the IUIProfileWindow object to be removed
             */
            virtual void removeProfile( SmartPtr<IUIProfileWindow> profile ) = 0;

            /**
             * @brief Gets the list of profile windows
             * @return An array of shared pointers to IUIProfileWindow objects
             */
            virtual Array<SmartPtr<IUIProfileWindow>> getProfiles() const = 0;

            /**
             * @brief Sets the list of profile windows
             * @param profiles An array of shared pointers to IUIProfileWindow objects
             */
            virtual void setProfiles( const Array<SmartPtr<IUIProfileWindow>> &profiles ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIProfilerWindow_h__
