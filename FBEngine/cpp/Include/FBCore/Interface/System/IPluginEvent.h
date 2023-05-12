#ifndef IPluginEvent_h__
#define IPluginEvent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    namespace core
    {
        /** Interface for a plugin event. */
        class IPluginEvent : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IPluginEvent() override = default;

            /** Gets the type of event. */
            virtual String getType() const = 0;

            /** Sets the type of event. */
            virtual void setType( const String &type ) = 0;

            /** Gets an argument. */
            virtual String getArgument( size_t index ) const = 0;

            /** Sets an argument. */
            virtual void setArgument( size_t index, const String &value ) = 0;
        };
    }  // namespace core
}  // namespace fb

#endif  // IPluginEvent_h__
