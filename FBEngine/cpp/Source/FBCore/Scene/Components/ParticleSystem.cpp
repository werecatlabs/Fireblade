#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ParticleSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Particle/IParticleSystem.h>
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
            auto applicationManager = core::ApplicationManager::instance();
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
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ParticleSystem::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        return {};
    }

    auto ParticleSystem::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setProperty( "lifetime", m_lifetime );
        return properties;
    }

    void ParticleSystem::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "lifetime", m_lifetime );
    }

    auto ParticleSystem::getGraphicsObject() const -> SmartPtr<render::IGraphicsObject>
    {
        return m_graphicsObject;
    }

    void ParticleSystem::setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject )
    {
        m_graphicsObject = graphicsObject;
    }

    auto ParticleSystem::getGraphicshNode() const -> SmartPtr<render::ISceneNode>
    {
        return m_graphicshNode;
    }

    void ParticleSystem::setGraphicshNode( SmartPtr<render::ISceneNode> graphicshNode )
    {
        m_graphicshNode = graphicshNode;
    }
}  // namespace fb::scene
