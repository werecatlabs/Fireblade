#ifndef __CCubemapOgreNext_h__
#define __CCubemapOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/ICubemap.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <OgreFrameListener.h>
//#include <OgreRenderTargetListener.h>

namespace fb
{
    namespace render
    {
        
        class CCubemapOgreNext : public ICubemap
        {
        public:
            CCubemapOgreNext();
            ~CCubemapOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
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

            void addExludedObject( SmartPtr<IGraphicsObject> object ) ;
            Array<SmartPtr<IGraphicsObject>> getExludedObjects() const;

            void generateMaterial( const String &materialName );

        protected:
            class CubemapFrameListener : public Ogre::FrameListener
            {
            public:
                CubemapFrameListener( CCubemapOgreNext *cubemap );
                ~CubemapFrameListener() override;

                bool frameEnded( const Ogre::FrameEvent &evt ) override;

                bool frameStarted( const Ogre::FrameEvent &evt ) override;

                bool frameRenderingQueued( const Ogre::FrameEvent &evt ) override;

            protected:
                CCubemapOgreNext *m_cubemap = nullptr;
            };

            class CubemapRTListener //: public Ogre::RenderTargetListener
            {
            public:
                CubemapRTListener( CCubemapOgreNext *cubemap );
                ~CubemapRTListener();

                // virtual void preViewportUpdate( const Ogre::RenderTargetViewportEvent& evt );

                // virtual void viewportAdded( const Ogre::RenderTargetViewportEvent& evt );

                // virtual void postViewportUpdate( const Ogre::RenderTargetViewportEvent& evt );

                // virtual void postRenderTargetUpdate( const Ogre::RenderTargetEvent& evt );

                // virtual void viewportRemoved( const Ogre::RenderTargetViewportEvent& evt );

                // virtual void preRenderTargetUpdate( const Ogre::RenderTargetEvent& evt );

            protected:
                CCubemapOgreNext *m_cubemap = nullptr;
            };

            Array<Ogre::Viewport *> m_viewports;
            Array<Ogre::RenderTarget *> m_renderTargets;

            Ogre::Camera *m_camera = nullptr;

            CubemapFrameListener *m_frameListener = nullptr;
            Ogre::SceneManager *m_sceneMgr = nullptr;
            // Ogre::TexturePtr m_cubemapTexture;
            Vector3F m_position;

            u32 m_visibilityMask = 0;
            u32 m_exclusionMask = 0;
            u32 m_currentIndex = 0;

            bool m_enable = false;

            String m_textureName;

            Array<SmartPtr<IGraphicsObject>> m_objects;
            Array<u32> m_oldMasks;

            static u32 m_nameExt;
        };
    } // end namespace render
}     // end namespace fb

#endif  // Cubemap_h__
