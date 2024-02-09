#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ParticleSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IParticleSystem.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, ParticleSystem, Component );

    ParticleSystem::ParticleSystem() = default;

    ParticleSystem::~ParticleSystem()
    {
        unload( nullptr );
    }

    void ParticleSystem::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                auto particleSystem = smgr->addGraphicsObjectByType<render::IParticleSystem>();

                auto rootNode = smgr->getRootSceneNode();
                auto node = rootNode->addChildSceneNode();
                node->attachObject( particleSystem );

                setParticleSystem( particleSystem );
            }

            setLoadingState( LoadingState::Loaded );
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
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                smgr->removeGraphicsObject( m_particleSystem );
                setParticleSystem( nullptr );
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    Array<SmartPtr<ISharedObject>> ParticleSystem::getChildObjects() const
    {
        Array<SmartPtr<ISharedObject>> childObjects;
        childObjects.push_back( m_particleSystem );
        childObjects.push_back( m_graphicsObject );
        childObjects.push_back( m_graphicsNode );
        return childObjects;
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

    SmartPtr<render::ISceneNode> ParticleSystem::getGraphicsNode() const
    {
        return m_graphicsNode;
    }

    void ParticleSystem::setGraphicsNode( SmartPtr<render::ISceneNode> graphicsNode )
    {
        m_graphicsNode = graphicsNode;
    }

    void ParticleSystem::setParticleSystem( SmartPtr<render::IParticleSystem> particleSystem )
    {
        m_particleSystem = particleSystem;
    }

    SmartPtr<render::IParticleSystem> ParticleSystem::getParticleSystem() const
    {
        return m_particleSystem;
    }
}  // namespace fb::scene
