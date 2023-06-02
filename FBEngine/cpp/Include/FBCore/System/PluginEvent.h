#ifndef PluginEvent_h__
#define PluginEvent_h__

#include <FBCore/Interface/System/IPluginEvent.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace core
    {
        
        class PluginEvent : public CSharedObject<IPluginEvent>
        {
        public:
            PluginEvent() = default;
            ~PluginEvent() override = default;

            /** @copydoc IPluginEvent::getType */
            String getType() const override;

            /** @copydoc IPluginEvent::setType */
            void setType( const String &type ) override;

            /** @copydoc IPluginEvent::getArgument */
            String getArgument( size_t index ) const override;

            /** @copydoc IPluginEvent::setArgument */
            void setArgument( size_t index, const String &value ) override;

            PluginEvent &operator=( const PluginEvent &other );

            const String &getArg1() const;
            void setArg1( const String &val );

            const String &getArg2() const;
            void setArg2( const String &val );

            const String &getArg3() const;
            void setArg3( const String &val );

            const String &getArg4() const;
            void setArg4( const String &val );

        protected:
            static constexpr u32 NUM_ARGS = 4;

            FixedString<128> m_type;
            FixedArray<String, NUM_ARGS> m_data;
        };
    }  // namespace core
}  // namespace fb

#endif  // PluginEvent_h__
