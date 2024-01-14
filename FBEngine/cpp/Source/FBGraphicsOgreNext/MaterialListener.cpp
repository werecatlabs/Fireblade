#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/MaterialListener.h"
#include <Ogre.h>

namespace fb
{

    MaterialListener::MaterialListener()
    {
        // m_sceneWithOutTextures = Ogre::MaterialManager::getSingletonPtr()->create(
        //	"Ogre/Compositor/VolumetricSunLight/SceneWithOutTextures",
        //	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        // m_depthMaterial = Ogre::MaterialManager::getSingletonPtr()->create(
        //	"Ogre/DepthWrite",
        //	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        // Ogre::Technique* technique = m_depthMaterial->getTechnique(0);
        // Ogre::Pass* pass = technique->getPass(0);
        // pass->setDepthWriteEnabled(true);
        // pass->setDepthCheckEnabled(true);
        // pass->setColourWriteEnabled(false);
        // pass->setDepthFunction(Ogre::CompareFunction::CMPF_LESS);
    }

    MaterialListener::~MaterialListener() = default;

    auto MaterialListener::handleSchemeNotFound( unsigned short schemeIndex,
                                                 const Ogre::String &schemeName,
                                                 Ogre::Material *originalMaterial,
                                                 unsigned short lodIndex, const Ogre::Renderable *rend )
        -> Ogre::Technique *
    {
        // if(schemeName == "depth")
        //{
        //	return m_depthMaterial->getTechnique(0);
        // }
        // else if(schemeName == "colour")
        //{
        //	Ogre::Technique* tech = originalMaterial->getTechnique(0);
        //	tech->setDepthWriteEnabled(false);
        //	//tech->setDepthFunction(Ogre::CompareFunction::CMPF_LESS_EQUAL);
        //	tech->setDepthBias(1.0, 0.0);
        //	return tech;
        // }
        // else if(schemeName == "VolumetricSunLightSceneWithOutTextures")
        //{
        //	return m_sceneWithOutTextures->getTechnique(0);
        // }

        return nullptr;
    }

}  // end namespace fb
