#ifndef _CResourceGroupManager_H
#define _CResourceGroupManager_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceGroupManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/Set.h>

namespace fb
{
    namespace render
    {
        class CResourceGroupManager : public SharedObject<IResourceGroupManager>
        {
        public:
            static const u32 RGMID_UNLOADRESOURCEGROUP;

            CResourceGroupManager();
            ~CResourceGroupManager() override;

            /** @copydoc IResourceGroupManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResourceGroupManager::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialiseAllResourceGroups() override;
            void initialiseResourceGroup( const String &groupName ) override;
            void unloadResourceGroup( const String &groupName ) override;
            void clearResourceGroup( const String &groupName ) override;
            void destroyResourceGroup( const String &groupName ) override;

            void _getObject( void **ppObject ) const override;

            /** */
            void reloadResources( const String &groupName ) override;

            /** */
            void parseScripts( const Array<String> &scripts ) override;
            void parseScripts( const Set<String> &scripts );

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

        protected:
            void parseScripts( Ogre::ResourceManager *resMgr, const String &extension,
                               const String &group );
            void parseScripts( Ogre::ParticleSystemManager *resMgr, const String &extension,
                               const String &group );
            void parseScripts( Ogre::ScriptLoader *loader, const String &extension,
                               const String &group );

            void parseScript( Ogre::ScriptLoader *loader, const String &fileName, const String &group );

            ResourceGroupHelper *m_resourceGroupHelper = nullptr;
            Ogre::ResourceGroupManager *m_resourceGroupManager = nullptr;

            AtomicSmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
        };
    }  // end namespace render
}  // end namespace fb

#endif
