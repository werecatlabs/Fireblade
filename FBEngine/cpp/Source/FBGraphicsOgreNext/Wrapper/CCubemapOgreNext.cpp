#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CCubemapOgreNext.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <OgreRoot.h>
#include <OgreViewport.h>

namespace fb
{
    namespace render
    {
        u32 CCubemapOgreNext::m_nameExt = 0;

        CCubemapOgreNext::CCubemapOgreNext() : m_currentIndex( 0 ), m_enable( false )
        {
        }

        CCubemapOgreNext::~CCubemapOgreNext()
        {
        }

        void CCubemapOgreNext::render()
        {
            for( u32 i = 0; i < m_renderTargets.size(); ++i )
            {
                // m_renderTargets[i]->update();
            }
        }

        void CCubemapOgreNext::setVisibilityMask( u32 visibilityMask )
        {
            // for(u32 i=0; i<m_viewports.size(); ++i)
            //	m_viewports[i]->setVisibilityMask(visibilityMask);

            m_visibilityMask = visibilityMask;
        }

        u32 CCubemapOgreNext::getExclusionMask() const
        {
            return m_exclusionMask;
        }

        void CCubemapOgreNext::setExclusionMask( u32 exclusionMask )
        {
            m_exclusionMask = exclusionMask;
        }

        SmartPtr<IGraphicsScene> CCubemapOgreNext::getSceneManager() const
        {
            return nullptr;
        }

        void CCubemapOgreNext::setSceneManager( SmartPtr<IGraphicsScene> smgr )
        {
            // smgr->_getObject((void**)&m_sceneMgr);

            // m_camera = m_sceneMgr->createCamera((String("CubemapCamera") +
            // StringUtil::toString(m_nameExt++)).c_str());
            // m_camera->setFOVy(Ogre::Radian(Ogre::Degree(90.0f)));
            // m_camera->setAspectRatio(1.0f);
            // m_camera->setFixedYawAxis(false);
            // m_camera->setNearClipDistance(0.001f);
            // m_camera->setFarClipDistance(10000.0f);

            ////Ogre::TextureManager* textureManager = Ogre::TextureManager::getSingletonPtr();
            ////m_cubemapTexture = textureManager->createManual(m_textureName.c_str(),
            /// Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, /	Ogre::TEX_TYPE_CUBE_MAP, 256,
            /// 256, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

            // m_renderTargets.reserve(6);
            // m_viewports.reserve(6);

            ////for(u32 i=0; i<6; ++i)
            ////{
            ////	Ogre::RenderTarget* renderTarget = m_cubemapTexture->getBuffer(i)->getRenderTarget();
            ////	m_renderTargets.push_back(renderTarget);

            ////	Ogre::Viewport* vp = renderTarget->addViewport(m_camera);
            ////	vp->setOverlaysEnabled(false);
            ////	vp->setAutoUpdated(false);
            ////	vp->setBackgroundColour(Ogre::ColourValue::Red);
            ////	vp->setClearEveryFrame(false);
            ////	m_viewports.push_back(vp);

            ////	renderTarget->addListener(new Cubemap::CubemapRTListener(this));
            ////}

            // m_frameListener = new Cubemap::CubemapFrameListener(this);
        }

        void CCubemapOgreNext::setUpdateInterval( u32 milliseconds )
        {
        }

        void CCubemapOgreNext::addExludedObject( SmartPtr<IGraphicsObject> object )
        {
            m_objects.push_back( object );
        }

        Array<SmartPtr<IGraphicsObject>> CCubemapOgreNext::getExludedObjects() const
        {
            return m_objects;
        }

        void CCubemapOgreNext::generateMaterial( const String &materialName )
        {
            // Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->create(
            //	materialName.c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            // Ogre::Technique* technique = material->getTechnique(0);
            // if (technique)
            //{
            //	Ogre::Pass* pass = technique->getPass(0);
            //	if (pass)
            //	{
            //		pass->setAmbient(Ogre::ColourValue(0.5, 0.5, 0.5, 1.0));
            //		pass->setDiffuse(Ogre::ColourValue(0.5, 0.5, 0.5, 1.0));

            //		Ogre::TextureUnitState* texState =
            // pass->createTextureUnitState(m_textureName.c_str());
            //		//texState->setCubicTextureName(m_cubemapTexture->getName(), false);
            //		//texState->set
            //		//texState->setContentType(Ogre::TextureUnitState::CONTENT_NAMED);
            //		texState->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_REFLECTION);
            //	}
            //}
        }

        u32 CCubemapOgreNext::getVisibilityMask() const
        {
            return m_visibilityMask;
        }

        Vector3F CCubemapOgreNext::getPosition() const
        {
            return m_position;
        }

        void CCubemapOgreNext::setPosition( const Vector3F &position )
        {
            m_position = position;
        }

        bool CCubemapOgreNext::getEnable() const
        {
            return m_enable;
        }

        void CCubemapOgreNext::setEnable( bool enable )
        {
            if( m_enable != enable )
            {
                m_enable = enable;

                if( m_enable )
                    Ogre::Root::getSingletonPtr()->addFrameListener( m_frameListener );
                else
                    Ogre::Root::getSingletonPtr()->removeFrameListener( m_frameListener );

                // for(u32 i=0; i<m_viewports.size(); ++i)
                //	m_viewports[i]->setAutoUpdated(m_enable);
            }
        }

        u32 CCubemapOgreNext::getUpdateInterval() const
        {
            return 0;
        }

        String CCubemapOgreNext::getTextureName() const
        {
            return m_textureName;
        }

        void CCubemapOgreNext::setTextureName( const String &textureName )
        {
            m_textureName = textureName;
        }

        void CCubemapOgreNext::load( SmartPtr<ISharedObject> data )
        {
        }

        void CCubemapOgreNext::unload( SmartPtr<ISharedObject> data )
        {
        }

        CCubemapOgreNext::CubemapFrameListener::CubemapFrameListener( CCubemapOgreNext *cubemap ) : m_cubemap( cubemap )
        {
        }

        CCubemapOgreNext::CubemapFrameListener::~CubemapFrameListener()
        {
        }

        bool CCubemapOgreNext::CubemapFrameListener::frameEnded( const Ogre::FrameEvent &evt )
        {
            return true;
        }

        bool CCubemapOgreNext::CubemapFrameListener::frameStarted( const Ogre::FrameEvent &evt )
        {
            if( m_cubemap->m_enable )
            {
                // for(u32 i=0; i<m_cubemap->m_renderTargets.size(); ++i)
                {
                    // m_cubemap->m_viewports[m_cubemap->m_currentIndex]->clear();
                    // m_cubemap->m_renderTargets[i]->update();
                    // m_cubemap->m_viewports[m_cubemap->m_currentIndex]->update();
                    ++m_cubemap->m_currentIndex;

                    if( m_cubemap->m_currentIndex > 5 )
                        m_cubemap->m_currentIndex = 0;
                }

                // m_cubemap->m_renderTargets[0]->writeContentsToFile("face1.png");
            }

            return true;
        }

        bool CCubemapOgreNext::CubemapFrameListener::frameRenderingQueued( const Ogre::FrameEvent &evt )
        {
            return true;
        }

        // void Cubemap::CubemapRTListener::preViewportUpdate( const Ogre::RenderTargetViewportEvent& evt
        // )
        //{
        // }

        // void Cubemap::CubemapRTListener::viewportAdded( const Ogre::RenderTargetViewportEvent& evt )
        //{
        // }

        // void Cubemap::CubemapRTListener::postViewportUpdate( const Ogre::RenderTargetViewportEvent&
        // evt )
        //{
        // }

        // void Cubemap::CubemapRTListener::postRenderTargetUpdate( const Ogre::RenderTargetEvent& evt )
        //{
        //	//for(u32 i=0; i<m_cubemap->m_objects.size() && i<m_cubemap->m_oldMasks.size(); ++i)
        //	for(u32 i=0; i<m_cubemap->m_objects.size(); ++i)
        //	{
        //		//m_cubemap->m_objects[i]->setVisibilityFlags(m_cubemap->m_oldMasks[i]);
        //		m_cubemap->m_objects[i]->setVisible(true);
        //	}
        // }

        // void Cubemap::CubemapRTListener::viewportRemoved( const Ogre::RenderTargetViewportEvent& evt )
        //{
        // }

        // void Cubemap::CubemapRTListener::preRenderTargetUpdate( const Ogre::RenderTargetEvent& evt )
        //{
        //	if(!m_cubemap->m_enable)
        //		return;

        //	m_cubemap->m_oldMasks.clear();

        //	for(u32 i=0; i<m_cubemap->m_objects.size(); ++i)
        //	{
        //		//m_cubemap->m_oldMasks.push_back(m_cubemap->m_objects[i]->getVisibilityFlags());
        //		//m_cubemap->m_objects[i]->setVisibilityFlags(m_cubemap->getExclusionMask());
        //		m_cubemap->m_objects[i]->setVisible(false);
        //	}

        //	m_cubemap->m_camera->setOrientation(Ogre::Quaternion::IDENTITY);

        //	Vector3F position = m_cubemap->getPosition();
        //	m_cubemap->m_camera->setPosition(Ogre::Vector3(position[0], position[1], position[2]));

        //	if(evt.source == m_cubemap->m_renderTargets[0])
        //		m_cubemap->m_camera->yaw(Ogre::Radian(Ogre::Degree(-90.0f)));
        //	else if(evt.source == m_cubemap->m_renderTargets[1])
        //		m_cubemap->m_camera->yaw(Ogre::Radian(Ogre::Degree(90.0f)));
        //	else if(evt.source == m_cubemap->m_renderTargets[2])
        //		m_cubemap->m_camera->pitch(Ogre::Radian(Ogre::Degree(90.0f)));
        //	else if(evt.source == m_cubemap->m_renderTargets[3])
        //		m_cubemap->m_camera->pitch(Ogre::Radian(Ogre::Degree(-90.0f)));
        //	//else if(evt.source == m_cubemap->m_renderTargets[4])
        //	//	m_cubemap->m_camera->yaw(Ogre::Degree(90.0f).valueRadians());
        //	else if(evt.source == m_cubemap->m_renderTargets[5])
        //		m_cubemap->m_camera->yaw(Ogre::Radian(Ogre::Degree(180.0f)));
        //}

        CCubemapOgreNext::CubemapRTListener::~CubemapRTListener()
        {
        }

        CCubemapOgreNext::CubemapRTListener::CubemapRTListener( CCubemapOgreNext *cubemap ) : m_cubemap( cubemap )
        {
        }
    }  // namespace render
}  // end namespace fb
