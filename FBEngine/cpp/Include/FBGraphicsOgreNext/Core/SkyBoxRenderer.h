#ifndef SkyBoxRenderer_h__
#define SkyBoxRenderer_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBGraphicsOgreNext/Core/SkyRenderer.h>
#include <OgreQuaternion.h>
#include <OgreMaterial.h>
#include <OgreSceneManager.h>

namespace fb
{
    namespace render
    {
        //--------------------------------------------
        class SkyBoxRenderer : public SkyRenderer
        {
        public:
            SkyBoxRenderer() = default;
            ~SkyBoxRenderer();

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

	        f32 getDistance() const;

            void setDistance( f32 distance );

	        Ogre::MaterialPtr getSkyMaterial() const;
            void setSkyMaterial( Ogre::MaterialPtr skyMaterial );

        protected:
            f32 m_distance = 1.0f;
            Ogre::Quaternion m_orientation = Ogre::Quaternion::IDENTITY;
            Ogre::ManualObject *m_manualObject = nullptr;
            Ogre::SceneNode *m_sceneNode = nullptr;
            Ogre::MaterialPtr m_skyMaterial;
            Ogre::SceneManager::SkyMethod mSkyMethod = Ogre::SceneManager::SkyMethod::SkyCubemap;
            Ogre::Rectangle2D *mSky = nullptr;
        };

    }  // namespace render
}  // namespace fb

#endif // SkyBoxRenderer_h__
