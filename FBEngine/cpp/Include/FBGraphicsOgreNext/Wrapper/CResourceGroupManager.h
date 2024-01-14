#ifndef _CResourceGroupManager_H
#define _CResourceGroupManager_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>
#include <FBCore/Interface/Resource/IResourceGroupManager.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Set.h>
#include <FBCore/System/Job.h>
#include <OgreArchive.h>

namespace fb
{
    namespace render
    {

        class CResourceGroupManager : public SharedGraphicsObject<IResourceGroupManager>
        {
        public:
            static const u32 RGMID_UNLOADRESOURCEGROUP;

            CResourceGroupManager();
            ~CResourceGroupManager() override;

            /** @copydoc IResourceGroupManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResourceGroupManager::initialiseAllResourceGroups */
            void initialiseAllResourceGroups() override;

            /** @copydoc IResourceGroupManager::initialiseResourceGroup */
            void initialiseResourceGroup( const String &groupName ) override;

            /** @copydoc IResourceGroupManager::unloadResourceGroup */
            void unloadResourceGroup( const String &groupName ) override;

            /** @copydoc IResourceGroupManager::clearResourceGroup */
            void clearResourceGroup( const String &groupName ) override;

            /** @copydoc IResourceGroupManager::destroyResourceGroup */
            void destroyResourceGroup( const String &groupName ) override;

            /** @copydoc IResourceGroupManager::_getObject */
            void _getObject( void **ppObject ) const override;

            /** @copydoc IResourceGroupManager::reloadResources */
            void reloadResources( const String &groupName ) override;

            /** @copydoc IResourceGroupManager::parseScripts */
            void parseScripts( const Array<String> &scripts ) override;

            /** @copydoc IResourceGroupManager::parseScripts */
            void parseScripts( const Set<String> &scripts );

            FB_CLASS_REGISTER_DECL;

        protected:
            class ResourceLoadJob : public Job
            {
            public:
                ResourceLoadJob();
                ~ResourceLoadJob() override;

                void execute() override;
                void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

                CResourceGroupManager *getResourceGroupManager() const;
                void setResourceGroupManager( CResourceGroupManager *val );

            private:
                CResourceGroupManager *m_resourceGroupManager = nullptr;
            };

            void parseScripts( const String &extension, const String &group );
            void parseScripts( Ogre::ResourceManager *resMgr, const String &extension,
                               const String &group );
            void parseScripts( Ogre::ParticleSystemManager *resMgr, const String &extension,
                               const String &group );
            void parseScripts( Ogre::ScriptLoader *loader, const String &extension,
                               const String &group );

            void parseScript( Ogre::ScriptLoader *loader, const String &fileName, const String &group );

            void loadResourceFile();

            void baseRegisterHlms();
            void registerHlms( void );

            void setupResources();

            void loadTextureCache( void );
            void saveTextureCache( void );
            void loadHlmsDiskCache( void );
            void saveHlmsDiskCache( void );

            ResourceGroupHelper *m_resourceGroupHelper = nullptr;

            String mPluginsFolder;
            String mWriteAccessFolder;
            String mResourcePath;

            bool mAlwaysAskForConfig = false;
            bool mUseHlmsDiskCache = false;
            bool mUseMicrocodeCache = false;

            SharedPtr<Ogre::Archive> m_fileSystemArchive;
        };
    }  // end namespace render
}  // end namespace fb

#endif
