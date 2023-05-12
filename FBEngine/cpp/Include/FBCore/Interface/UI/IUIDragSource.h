#ifndef IUIDragSource_h__
#define IUIDragSource_h__

#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIDragSource
         * @brief Interface for a drag source, responsible for handling drag events in the user interface
         */
        class IUIDragSource : public IEventListener
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIDragSource() override = default;

            /**
             * @brief Handles a drag event
             * @param position The position of the drag event in the UI
             * @param element The UI element involved in the drag event
             * @return The data associated with the drag event as a string
             */
            virtual String handleDrag( const Vector2I &position, SmartPtr<IUIElement> element ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIDragSource_h__
