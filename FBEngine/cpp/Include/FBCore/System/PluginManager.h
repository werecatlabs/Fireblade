#ifndef __PluginManager_h__
#define __PluginManager_h__

#include <FBCore/Interface/System/IPluginManager.h>

namespace fb
{
    namespace core
    {

        /** Implementation for a plugin manager. */
        class PluginManager : public IPluginManager
        {
        public:
            /** Constructor. */
            PluginManager() = default;

            /** Destructor. */
            ~PluginManager() override = default;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IPluginManager::loadPlugin */
            SmartPtr<IPlugin> loadPlugin( const String &filename ) override;

            /** @copydoc IPluginManager::loadPlugin */
            void loadPlugin( SmartPtr<IPlugin> plugin ) override;

            /** @copydoc IPluginManager::unloadPlugin */
            void unloadPlugin( SmartPtr<IPlugin> plugin ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Array of loaded plugins. */
            Array<SmartPtr<ISharedObject>> m_plugins;

            /** Mutex for thread safety. */
            mutable RecursiveMutex m_mutex;
        };

    }  // namespace core
}  // namespace fb

#endif  // PluginManager_h__
