#ifndef __PluginManager_h__
#define __PluginManager_h__

#include <FBCore/Interface/System/IPluginManager.h>


namespace fb
{
    namespace core
    {
        
        class PluginManager : public IPluginManager
        {
        public:
            PluginManager() = default;
            ~PluginManager() override = default;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IPlugin> loadPlugin( const String &filename ) override;

            void unloadPlugin( SmartPtr<IPlugin> plugin ) override;
        };

    }  // namespace core
}  // namespace fb

#endif  // PluginManager_h__
