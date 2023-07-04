#ifndef __CCubemap_h__
#define __CCubemap_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ICubemap.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/Array.h>
#include <OgreFrameListener.h>
#include <OgreRenderTargetListener.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        /**
        * @brief A cubemap object.
         */
        class CCubemap : public SharedObject<ICubemap>
        {
        public:
            /** @brief Constructor. */
            CCubemap();

            /** @brief Destructor. */
            ~CCubemap() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            String getTextureName() const override;
            void setTextureName( const String &textureName ) override;

            void render();

            void setVisibilityMask( u32 visibilityMask ) override;

            u32 getExclusionMask() const override;
            void setExclusionMask( u32 exclusionMask ) override;

            SmartPtr<IGraphicsScene> getSceneManager() const override;

            void setSceneManager( SmartPtr<IGraphicsScene> smgr ) override;

            u32 getVisibilityMask() const override;

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &position ) override;

            bool getEnable() const override;
            void setEnable( bool enable ) override;

            u32 getUpdateInterval() const override;
            void setUpdateInterval( u32 milliseconds ) override;

            void addExcludedObject( SmartPtr<IGraphicsObject> object ) override;
            Array<SmartPtr<IGraphicsObject>> getExcludedObjects() const override;

            void generateMaterial( const String &materialName );

            FB_CLASS_REGISTER_DECL;

        protected:
            class CubemapFrameListener : public Ogre::FrameListener
            {
            public:
                CubemapFrameListener( CCubemap *cubemap );
                ~CubemapFrameListener() override;

                bool frameEnded( const Ogre::FrameEvent &evt ) override;

                bool frameStarted( const Ogre::FrameEvent &evt ) override;

                bool frameRenderingQueued( const Ogre::FrameEvent &evt ) override;

            protected:
                CCubemap *m_cubemap = nullptr;
            };

            class CubemapRTListener : public Ogre::RenderTargetListener
            {
            public:
                CubemapRTListener( CCubemap *cubemap );
                ~CubemapRTListener() override;

                void preViewportUpdate( const Ogre::RenderTargetViewportEvent &evt ) override;

                void viewportAdded( const Ogre::RenderTargetViewportEvent &evt ) override;

                void postViewportUpdate( const Ogre::RenderTargetViewportEvent &evt ) override;

                void postRenderTargetUpdate( const Ogre::RenderTargetEvent &evt ) override;

                void viewportRemoved( const Ogre::RenderTargetViewportEvent &evt ) override;

                void preRenderTargetUpdate( const Ogre::RenderTargetEvent &evt ) override;

            protected:
                CCubemap *m_cubemap = nullptr;
            };

            Ogre::Camera *m_camera = nullptr;
            CubemapFrameListener *m_frameListener = nullptr;
            Ogre::SceneManager *m_sceneMgr = nullptr;

            Ogre::TexturePtr m_cubemapTexture;

            Vector3F m_position;

            u32 m_visibilityMask = 0;
            u32 m_exclusionMask = 0;
            u32 m_currentIndex = 0;

            bool m_enable = true;

            String m_textureName;

            Array<Ogre::Viewport *> m_viewports;
            Array<Ogre::RenderTarget *> m_renderTargets;
            Array<SmartPtr<IGraphicsObject>> m_objects;
            Array<u32> m_oldMasks;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // __CCubemap_h__
