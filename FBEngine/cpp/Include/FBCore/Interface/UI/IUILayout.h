#ifndef _IGUILayout_H
#define _IGUILayout_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUILayout
         * @brief Interface for a UI layout element, responsible for managing layout elements in the user interface
         */
        class IUILayout : public IUIElement
        {
        public:
            /**
             * @enum LayoutStates
             * @brief Enumerates the different layout states
             */
            enum class LayoutStates
            {
                FS_IDLE,     ///< Idle state
                FS_FADEIN,   ///< Fade-in state
                FS_FADEOUT,  ///< Fade-out state

                FS_COUNT  ///< Number of layout states
            };

            /**
             * @brief Destructor
             */
            ~IUILayout() override = default;

            /**
             * @brief Retrieves the Finite State Machine (FSM) associated with the layout
             * @return A SmartPtr to an IFSM object representing the layout's FSM
             */
            virtual SmartPtr<IFSM> getFSM() = 0;

            /**
             * @brief Gets the UI window associated with the layout
             * @return A SmartPtr to an IUIWindow object representing the UI window
             */
            virtual SmartPtr<IUIWindow> getUiWindow() const = 0;

            /**
             * @brief Sets the UI window associated with the layout
             * @param uiWindow A SmartPtr to an IUIWindow object representing the UI window
             */
            virtual void setUiWindow( SmartPtr<IUIWindow> uiWindow ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
