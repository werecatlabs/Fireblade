#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ParticleSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Particle/IParticleSystem.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, ParticleSystem, Component );

        ParticleSystem::ParticleSystem()
        {
        }

        ParticleSystem::~ParticleSystem()
        {
            unload( nullptr );
        }

        void ParticleSystem::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    auto particleSystem = smgr->addGraphicsObjectByType<render::IParticleSystem>();
                    auto node = smgr->getRootSceneNode()->addChildSceneNode();
                    node->attachObject( particleSystem );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ParticleSystem::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
#if FB_USE_PARTICLE_UNIVERSE
                if( m_smokeObject )
                {
                    ParticleUniverse::ParticleSystem *smoke =
                        m_smokeObject->getObject<ParticleUniverse::ParticleSystem>();
                    if( smoke )
                    {
                        s32 numTechniques = smoke->getNumTechniques();
                        if( numTechniques > 0 )
                        {
                            ParticleUniverse::ParticleTechnique *tech = smoke->getTechnique( 0 );
                            if( tech )
                            {
                                tech->removeExtern( m_particleGroundEffect );
                            }
                        }

                        state::Ogre::SceneManager smgr = m_smokeNode->getCreator();

                        ParticleUniverse::ParticleSystemManager *psMgr =
                            ParticleUniverse::ParticleSystemManager::getSingletonPtr();
                        psMgr->destroyParticleSystem( smoke->getName(), smgr->getSceneManager() );
                        m_smokeObject->setMovableObject( nullptr );
                    }
                }

                if( m_smokeNode )
                {
                    m_smokeNode->detachAllObjects();
                    m_smokeNode->getCreator()->destroySceneNode( m_smokeNode );
                    m_smokeNode = nullptr;
                }

                FB_SAFE_DELETE( m_particleGroundEffect );
#endif
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<ISharedObject>> ParticleSystem::getChildObjects() const
        {
            return Array<SmartPtr<ISharedObject>>();
        }

        SmartPtr<Properties> ParticleSystem::getProperties() const
        {
            auto properties = Component::getProperties();
            properties->setProperty( "lifetime", m_lifetime );
            return properties;
        }

        void ParticleSystem::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "lifetime", m_lifetime );
        }

        SmartPtr<render::IGraphicsObject> ParticleSystem::getGraphicsObject() const
        {
            return m_graphicsObject;
        }

        void ParticleSystem::setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject )
        {
            m_graphicsObject = graphicsObject;
        }

        SmartPtr<render::ISceneNode> ParticleSystem::getGraphicshNode() const
        {
            return m_graphicshNode;
        }

        void ParticleSystem::setGraphicshNode( SmartPtr<render::ISceneNode> graphicshNode )
        {
            m_graphicshNode = graphicshNode;
        }
    }  // namespace scene
}  // end namespace fb
