#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/FileListener.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Script/IScriptManager.h>

namespace fb
{
    FileListener::FileListener()
    {
    }

    FileListener::~FileListener()
    {
    }

    //--------------------------------------------
    // void FileListener::handleFileAction( FW::WatchID watchid, const FW::String& dirStr, const
    // FW::String& filenameStr, FW::Action action )
    //{
    //	auto applicationManager = core::IApplicationManager::instance();

    //	String fileName = filenameStr.c_str();
    //	String ext = FileSystem::getFileExtension(fileName);
    //	if(ext==(".fdb"))
    //	{
    //		/*Sleep(200);
    //		for(u32 i=0; i<DATABASE_COUNT; ++i)
    //		{
    //			DatabaseMgrPtr database;// =
    //IApplicationManager::instance()->getGameSceneManager()->getDatabase(i);
    //			database->handleResourceModified(fileName);
    //		}*/
    //	}
    //	else if(ext==(".lua"))
    //	{
    //		Thread::sleep(200);
    //		reloadLuaScripts();

    //	}
    //	else if(ext==(".gui"))
    //	{
    //		//RecursiveMutex::ScopedLock lock(OgreMutex);
    //		//CGUIManager::getSingletonPtr()->reloadCurrentLayout();
    //	}
    //	else if(ext==(".cg") || ext==(".hlsl") || ext==(".glsl"))
    //	{
    //		//RecursiveMutex::ScopedLock lock(OgreMutex);
    //		////IApplicationManager::instance()->getGraphicsSystem()->reloadResources("material");

    //		//reloadAResourceGroupWithoutDestroyingIt("gpuProgram");

    //		//parseScripts(Ogre::MaterialManager::getSingletonPtr(), ".program", "gpuProgram");

    //		//ResourceGroupHelper resGrpHelper;
    //		//resGrpHelper.updateOnEveryRenderable();
    //	}
    //	else if(ext==(".material"))
    //	{
    //		//RecursiveMutex::ScopedLock lock(OgreMutex);
    //		////IApplicationManager::instance()->getGraphicsSystem()->reloadResources("material");

    //		//reloadAResourceGroupWithoutDestroyingIt("material");

    //		//parseScripts(Ogre::MaterialManager::getSingletonPtr(), ".material", "material");

    //		//ResourceGroupHelper resGrpHelper;
    //		//resGrpHelper.updateOnEveryRenderable();
    //	}
    //	else if(ext==(".pu"))
    //	{
    //		reloadParticles();
    //	}
    //	else if(ext==(".dds") ||
    //			ext==(".tga") ||
    //			ext==(".png") ||
    //			ext==(".jpg") ||
    //			ext==(".bmp") )
    //	{
    //		Thread::sleep(200);
    //		reloadTexture(fileName);
    //	}

    //	SmartPtr<IEntityManager>& entityManager = engine->getEntityManager();
    //	Array<SmartPtr<IActor>> entities;
    //	entityManager->getEntities(entities);
    //	for(u32 i=0; i<entities.size(); ++i)
    //	{
    //		SmartPtr<IActor>& entity = entities[i];
    //		//entity->handleResourceModified(fileName);
    //	}
    //}

    //--------------------------------------------
    void FileListener::reloadLuaScripts()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto scriptManager = applicationManager->getScriptManager();
        scriptManager->reloadScripts();
    }

    //--------------------------------------------
    void FileListener::reloadTexture( const String &fileName )
    {
        // Get the texture with this name (if it exists)
        // Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(fileName.c_str());
        //// if the texture exists, reload it
        // if(!tex.isNull())
        //	tex->reload();
    }

    //--------------------------------------------
    bool FileListener::reloadAResourceGroupWithoutDestroyingIt( const std::string &pResourceGroupName )
    {
        // if(!resourceGroupExist(pResourceGroupName))
        //{
        //	// not present. something wrong.
        //	return false;
        // }
        // Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
        // resGroupMgr.clearResourceGroup(pResourceGroupName);
        // resGroupMgr.initialiseResourceGroup(pResourceGroupName);
        return true;
    }

    //--------------------------------------------
    bool FileListener::resourceGroupExist( const std::string &pResourceGroupName )
    {
        // bool lIsPresent = false;
        // Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
        // Ogre::StringVector lAllResourceGroups = resGroupMgr.getResourceGroups();
        // Ogre::StringVector::iterator iter = lAllResourceGroups.begin();
        // Ogre::StringVector::iterator iterEnd = lAllResourceGroups.end();
        // for(;iter!=iterEnd;++iter)
        //{
        //	if((*iter) == pResourceGroupName)
        //	{
        //		lIsPresent = true;
        //	}
        // }
        // return lIsPresent;
        //

        return true;
    }

    //--------------------------------------------
    void FileListener::reloadParticles()
    {
        // ParticleUniverse::ParticleSystemManager* particleSysMgr =
        // ParticleUniverse::ParticleSystemManager::getSingletonPtr(); Array<Ogre::String> v;
        ////particleSysMgr->particleSystemTemplateNames(v);

        // for(u32 i=0; i<v.size(); ++i)
        //{
        //	//particleSysMgr->destroyParticleSystemTemplate(v[i]);
        // }

        // reloadAResourceGroupWithoutDestroyingIt("Particles");

        // SpecialEffectManager::getSingletonPtr()->reload();
    }
}  // end namespace fb
