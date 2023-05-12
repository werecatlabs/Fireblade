#include <FBCore/FBCorePCH.h>
#include <FBCore/System/PluginEvent.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Data/DefineDataStructures.h>

namespace fb
{
    namespace core
    {

        String PluginEvent::getType() const
        {
            return m_type.c_str();
        }

        void PluginEvent::setType( const String &type )
        {
            m_type = type;
        }

        String PluginEvent::getArgument( size_t index ) const
        {
            FB_ASSERT( index < NUM_ARGS );
            auto &str = m_data[index];
            return str;
        }

        void PluginEvent::setArgument( size_t index, const String &value )
        {
            FB_ASSERT( index < NUM_ARGS );
            m_data[index] = value;
        }

        PluginEvent &PluginEvent::operator=( const PluginEvent &other )
        {
            for( size_t i = 0; i < NUM_ARGS; ++i )
            {
                m_data[i] = other.m_data[i];
            }

            return *this;
        }

        String PluginEvent::toJson() const
        {
            auto data = toData();
            auto pData = data->getDataAsType<data::plugin_event>();
            return DataUtil::toString( pData );
        }

        SmartPtr<IData> PluginEvent::toData() const
        {
            auto pData = fb::make_ptr<Data<data::plugin_event>>();
            auto data = pData->getDataAsType<data::plugin_event>();

            data->type = getType();
            data->arg1 = getArgument( 0 );
            data->arg2 = getArgument( 1 );
            data->arg3 = getArgument( 2 );
            data->arg4 = getArgument( 3 );

            return pData;
        }

        void PluginEvent::fromData( SmartPtr<IData> data )
        {
            auto pData = data->getDataAsType<data::plugin_event>();
            m_type = pData->type;

            FB_ASSERT( m_data.size() < NUM_ARGS );

            m_data[0] = pData->arg1;
            m_data[1] = pData->arg2;
            m_data[2] = pData->arg3;
            m_data[3] = pData->arg4;
        }

        const String &PluginEvent::getArg1() const
        {
            return m_data[0];
        }

        void PluginEvent::setArg1( const String &val )
        {
            m_data[0] = val;
        }

        const String &PluginEvent::getArg2() const
        {
            return m_data[1];
        }

        void PluginEvent::setArg2( const String &val )
        {
            m_data[1] = val;
        }

        const String &PluginEvent::getArg3() const
        {
            return m_data[2];
        }

        void PluginEvent::setArg3( const String &val )
        {
            m_data[2] = val;
        }

        const String &PluginEvent::getArg4() const
        {
            return m_data[3];
        }

        void PluginEvent::setArg4( const String &val )
        {
            m_data[3] = val;
        }
    }  // namespace core
}  // namespace fb
