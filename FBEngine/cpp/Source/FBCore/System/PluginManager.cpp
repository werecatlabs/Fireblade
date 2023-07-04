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
            const auto filePathUTF16 = StringUtil::toUTF8to16( filename );

            auto plugin = fb::make_ptr<Plugin>();
            plugin->setFilePath( filePathUTF16 );

            return plugin;
        }

        void PluginManager::unloadPlugin( SmartPtr<IPlugin> plugin )
        {
        }
    }  // namespace core
}  // namespace fb
