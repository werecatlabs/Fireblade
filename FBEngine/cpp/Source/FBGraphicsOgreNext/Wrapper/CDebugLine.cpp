#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDebugLine.h>
#include <FBGraphicsOgreNext/DynamicLines.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/System/ITimer.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <Ogre.h>
#include <OgreHlmsUnlit.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreManualObject2.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, CDebugLine, IDebugLine );

        u32 CDebugLine::m_nameExt = 0;

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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                Ogre::SceneManager *sceneManager = nullptr;
                smgr->_getObject( (void **)&sceneManager );

                if( sceneManager )
                {
                    auto root = Ogre::Root::getSingletonPtr();
                    auto rootSceneNode = sceneManager->getRootSceneNode();
                    auto sceneNode = rootSceneNode->createChildSceneNode();

                    auto hlmsManager = root->getHlmsManager();

                    auto hlmsUnlitMaterials =
                        static_cast<Ogre::HlmsUnlit *>( hlmsManager->getHlms( Ogre::HLMS_UNLIT ) );
                    Ogre::HlmsUnlitDatablock *UnlitDatablock;

                    auto materialName = "UnlitColour" + StringUtil::getUUID();
                    setMaterialName( materialName );

                    UnlitDatablock =
                        static_cast<Ogre::HlmsUnlitDatablock *>( hlmsUnlitMaterials->createDatablock(
                            materialName, materialName, Ogre::HlmsMacroblock(), Ogre::HlmsBlendblock(),
                            Ogre::HlmsParamVec() ) );

                    UnlitDatablock->setUseColour( true );

                    auto iColour = getColour();

                    Ogre::ColourValue colourValue;
                    colourValue.setAsRGBA( iColour );
                    colourValue.a = 1.0f;

                    UnlitDatablock->setColour( colourValue );
                    setDatablock( UnlitDatablock );

                    auto manualObject = sceneManager->createManualObject( Ogre::SCENE_DYNAMIC );

                    //manualObject->colour( 1.0, 0, 0 );                     // tested but doesn't work
                    manualObject->setStatic( false );

                    sceneNode->attachObject( manualObject );

                    m_manualObject = manualObject;
                    m_sceneNode = sceneNode;
                }

                setLoadingState( LoadingState::Loaded );
            }
        }

        void CDebugLine::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    ScopedLock lock( graphicsSystem );

                    if( m_dynamicLines )
                    {
                        delete m_dynamicLines;
                        m_dynamicLines = nullptr;
                    }

                    if( m_sceneNode )
                    {
                        if( m_manualObject )
                        {
                            m_sceneNode->detachObject( m_manualObject );
                        }

                        auto sceneManager = m_sceneNode->getCreator();
                        if( sceneManager )
                        {
                            sceneManager->destroySceneNode( m_sceneNode );
                            m_sceneNode = nullptr;

                            if( m_manualObject )
                            {
                                sceneManager->destroyManualObject( m_manualObject );
                                m_manualObject = nullptr;
                            }
                        }
                    }

                    if( auto datablock = getDatablock() )
                    {
                        auto root = Ogre::Root::getSingletonPtr();
                        auto hlmsManager = root->getHlmsManager();

                        auto hlmsUnlitMaterials =
                            static_cast<Ogre::HlmsUnlit *>( hlmsManager->getHlms( Ogre::HLMS_UNLIT ) );
                        hlmsUnlitMaterials->destroyDatablock( datablock->getName() );

                        setDatablock( nullptr );
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebugLine::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            auto stateTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            if( task == stateTask )
            {
                if( m_isDirty )
                {
                    //load( nullptr );

                    if( m_dynamicLines )
                    {
                        m_dynamicLines->clear();

                        auto s = m_position;
                        auto e = m_position + m_vector;

                        auto p1 = Ogre::Vector3( s.X(), s.Y(), s.Z() );
                        auto p2 = Ogre::Vector3( e.X(), e.Y(), e.Z() );

                        // m_dynamicLines->addPoint( p1 );
                        // m_dynamicLines->addPoint( p2 );
                        // m_dynamicLines->update();
                    }

                    if( m_manualObject )
                    {
                        auto s = m_position;
                        auto e = m_position + m_vector;

                        auto p1 = Ogre::Vector3( s.X(), s.Y(), s.Z() );
                        auto p2 = Ogre::Vector3( e.X(), e.Y(), e.Z() );

                        //auto materialName = String( "UnlitColour" );
                        auto materialName = getMaterialName();

                        m_manualObject->clear();

                        m_manualObject->begin( materialName,
                                               Ogre::OT_LINE_LIST );  // Datablock DbRed works

                        m_manualObject->position( p1 );
                        m_manualObject->position( p2 );
                        m_manualObject->line( 0, 1 );
                        m_manualObject->end();
                    }

                    m_lifeTime = 0.0;
                    m_isDirty = false;
                }

                m_lifeTime += dt;
            }
        }

        Vector3F CDebugLine::getVector() const
        {
            return m_vector;
        }

        void CDebugLine::setVector( const Vector3F &vector )
        {
            m_vector = vector;
        }

        Vector3F CDebugLine::getPosition() const
        {
            return m_position;
        }

        void CDebugLine::setPosition( const Vector3F &position )
        {
            m_position = position;
        }

        f64 CDebugLine::getLifeTime() const
        {
            return m_lifeTime;
        }

        void CDebugLine::setLifeTime( f64 lifeTime )
        {
            m_lifeTime = lifeTime;
        }

        f64 CDebugLine::getMaxLifeTime() const
        {
            return m_maxLifeTime;
        }

        void CDebugLine::setMaxLifeTime( f64 maxLifeTime )
        {
            m_maxLifeTime = maxLifeTime;
        }

        bool CDebugLine::isVisible() const
        {
            return m_isVisible;
        }

        void CDebugLine::setVisible( bool visible )
        {
            m_isVisible = visible;
        }

        hash32 CDebugLine::getId() const
        {
            return m_id;
        }

        void CDebugLine::setId( hash32 id )
        {
            m_id = id;
        }

        Ogre::ObjectMemoryManager *CDebugLine::getMemoryManager() const
        {
            return m_memoryManager;
        }

        void CDebugLine::setMemoryManager( Ogre::ObjectMemoryManager *memoryManager )
        {
            m_memoryManager = memoryManager;
        }

        Ogre::HlmsUnlitDatablock *CDebugLine::getDatablock() const
        {
            return m_datablock;
        }

        void CDebugLine::setDatablock( Ogre::HlmsUnlitDatablock *datablock )
        {
            m_datablock = datablock;
        }

        String CDebugLine::getMaterialName() const
        {
            return m_materialName;
        }

        void CDebugLine::setMaterialName( const String &materialName )
        {
            m_materialName = materialName;
        }

        u32 CDebugLine::getColour() const
        {
            return m_colour;
        }

        void CDebugLine::setColour( u32 colour )
        {
            if( m_colour != colour )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                m_colour = colour;

                if( auto datablock = getDatablock() )
                {
                    auto iColour = getColour();
                    Ogre::ColourValue colourValue;
                    colourValue.setAsRGBA( iColour );
                    colourValue.a = 1.0f;

                    datablock->setColour( colourValue );
                }
            }
        }

    }  // end namespace render
}  // end namespace fb
