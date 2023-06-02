#ifndef _FB_IComponentEventListener_h__
#define _FB_IComponentEventListener_h__

#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace scene
    {
        /** Interface for an event. */
        class IComponentEventListener : public IEventListener
        {
        public:
            enum class Type
            {
                Loading,
                Object,
                UI,

                Count
            };

            /** Virtual destructor. */
            ~IComponentEventListener() override = default;

            /** Gets the component. */
            virtual SmartPtr<IComponent> getComponent() const = 0;

            /** Sets the component. */
            virtual void setComponent( SmartPtr<IComponent> component ) = 0;

            /** Gets the function. */
            virtual String getFunction() const = 0;

            /** Sets the function. */
            virtual void setFunction( const String &function ) = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IEvent_h__
