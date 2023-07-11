#ifndef IUIDropTarget_h__
#define IUIDropTarget_h__

#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIDropTarget
         * @brief Interface for a drop target, responsible for handling drop events in the user interface
         */
        class IUIDropTarget : public IEventListener
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIDropTarget() override = default;

            ///**
            // * @brief Handles a drop event
            // * @param position The position of the drop event in the UI
            // * @param src The source UI element involved in the drop event
            // * @param dst The destination UI element involved in the drop event
            // * @param data The data associated with the drop event
            // * @return true if the drop event is handled successfully, false otherwise
            // */
            //virtual bool handleDrop( const Vector2I &position, SmartPtr<IUIElement> src,
            //                         SmartPtr<IUIElement> dst, const String &data ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIDropTarget_h__
