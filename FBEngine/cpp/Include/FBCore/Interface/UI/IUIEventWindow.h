#ifndef IUIEventWindow_h__
#define IUIEventWindow_h__

#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace ui
    {

        class IUIEventWindow : public IUIWindow
        {
        public:
            /** Destructor. */
            ~IUIEventWindow() override = default;

            /** Gets the events. */
            virtual Array<SmartPtr<IEvent>> getEvents() const = 0;

            /** Sets the events. */
            virtual void setEvents( const Array<SmartPtr<IEvent>> &events ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIEvent_h__
