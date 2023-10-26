#include <FBCore/FBCorePCH.h>
#include <FBCore/System/PluginManager.h>
#include <FBCore/System/Plugin.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace core
    {

        void PluginManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void PluginManager::reload( SmartPtr<ISharedObject> data )
        {
        }

        void PluginManager::unload( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<IPlugin> PluginManager::loadPlugin( const String &filename )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            const auto filePathUTF16 = StringUtil::toUTF8to16( filename );

            auto plugin = fb::make_ptr<Plugin>();
            plugin->setFilePath( filePathUTF16 );

            m_plugins.push_back( plugin );

            return plugin;
        }

        void PluginManager::loadPlugin( SmartPtr<IPlugin> plugin )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_plugins.push_back( plugin );
        }

        void PluginManager::unloadPlugin( SmartPtr<IPlugin> plugin )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            m_plugins.erase( std::remove( m_plugins.begin(), m_plugins.end(), plugin ),
                             m_plugins.end() );
        }

    }  // namespace core
}  // namespace fb
