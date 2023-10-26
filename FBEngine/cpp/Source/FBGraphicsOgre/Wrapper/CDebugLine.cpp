#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDebugLine.h>
#include <FBGraphicsOgre/Ogre/DynamicLines.h>
#include <FBCore/FBCore.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, CDebugLine, IDebugLine );

        u32 CDebugLine::m_nameExt = 0;

        CDebugLine::CDebugLine()
        {
        }

        CDebugLine::~CDebugLine()
        {
            unload( nullptr );
        }

        void CDebugLine::load( SmartPtr<ISharedObject> data )
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Loading );

                ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
                ////printf("VisualForceVector: initialise()\n");
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto smgr = graphicsSystem->getGraphicsScene();

                Ogre::SceneManager *sceneManager = nullptr;
                smgr->_getObject( (void **)&sceneManager );

                m_dynamicLines = new DynamicLines();
                m_dynamicLines->setCastShadows( false );
                // m_dynamicLines->setMaterial("");

                m_materialName = "DebugLines_" + StringUtil::toString( m_nameExt++ );
                auto materialManager = Ogre::MaterialManager::getSingletonPtr();
                auto materialResult = materialManager->createOrRetrieve(
                    m_materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                m_material = materialResult.first.dynamicCast<Ogre::Material>();

                if( m_material->getNumTechniques() > 0 )
                {
                    auto technique = m_material->getTechnique( 0 );

                    if( technique->getNumPasses() > 0 )
                    {
                        auto pass = technique->getPass( 0 );
                        if( pass )
                        {
                            pass->setDepthCheckEnabled( false );
                            pass->setDepthWriteEnabled( false );
                        }
                    }
                }
                else
                {
                    auto technique = m_material->createTechnique();
                    if( technique )
                    {
                        auto pass = technique->createPass();
                        if( pass )
                        {
                            pass->setDepthCheckEnabled( false );
                            pass->setDepthWriteEnabled( false );
                        }
                    }
                }

                m_dynamicLines->setMaterial( m_material );
                m_dynamicLines->setRenderQueueGroup( Ogre::RENDER_QUEUE_SKIES_LATE );

                m_sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
                m_sceneNode->attachObject( m_dynamicLines );

                setLoadingState( LoadingState::Loaded );
            }
        }

        void CDebugLine::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( m_dynamicLines )
                {
                    delete m_dynamicLines;
                    m_dynamicLines = nullptr;
                }

                if( m_material )
                {
                    auto materialManager = Ogre::MaterialManager::getSingletonPtr();
                    materialManager->remove( m_material );
                    m_material = nullptr;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebugLine::preUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            switch( task )
            {
            case Thread::Task::Render:
            {
                if( m_isDirty )
                {
                    load( nullptr );

                    if( m_dynamicLines )
                    {
                        m_dynamicLines->clear();

                        auto s = m_position;
                        auto e = m_position + m_vector;

                        auto p1 = Ogre::Vector3( s.X(), s.Y(), s.Z() );
                        auto p2 = Ogre::Vector3( e.X(), e.Y(), e.Z() );

                        m_dynamicLines->addPoint( p1 );
                        m_dynamicLines->addPoint( p2 );
                        m_dynamicLines->update();
                    }

                    m_lifeTime = 0.0;
                    m_isDirty = false;
                }

                m_lifeTime += dt;
            }
            break;
            }
        }

        Vector3F CDebugLine::getVector() const
        {
            return m_vector;
        }

        void CDebugLine::setVector( const Vector3F &val )
        {
            m_vector = val;
        }

        Vector3F CDebugLine::getPosition() const
        {
            return m_position;
        }

        void CDebugLine::setPosition( const Vector3F &val )
        {
            m_position = val;
        }

        f64 CDebugLine::getLifeTime() const
        {
            return m_lifeTime;
        }

        void CDebugLine::setLifeTime( f64 val )
        {
            m_lifeTime = val;
        }

        f64 CDebugLine::getMaxLifeTime() const
        {
            return m_maxLifeTime;
        }

        void CDebugLine::setMaxLifeTime( f64 val )
        {
            m_maxLifeTime = val;
        }

        bool CDebugLine::isVisible() const
        {
            return m_isVisible;
        }

        void CDebugLine::setVisible( bool val )
        {
            m_isVisible = val;
        }

        hash32 CDebugLine::getId() const
        {
            return m_id;
        }

        void CDebugLine::setId( hash32 id )
        {
            m_id = id;
        }

        u32 CDebugLine::getColour() const
        {
            return m_colour;
        }

        void CDebugLine::setColour( u32 colour )
        {
            m_colour = colour;
        }

    }  // end namespace render
}  // end namespace fb
