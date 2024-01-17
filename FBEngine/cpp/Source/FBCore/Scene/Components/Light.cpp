#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Light.h>
#include <FBCore/Scene/Transform.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Light, IComponent );
    const hash_type Light::lightHash = StringUtil::getHash( "Light" );

    u32 Light::m_nameExt = 0;

    Light::Light() = default;

    Light::~Light()
    {
        unload( nullptr );
    }

    void Light::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Component::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto name = String( "Light" ) + StringUtil::toString( m_nameExt++ );

                auto light = smgr->addLight( name );
                light = light;

                light->setType( render::ILight::LightTypes::LT_DIRECTIONAL );
                setLight( light );

                auto rootSceneNode = smgr->getRootSceneNode();
                FB_ASSERT( rootSceneNode );

                auto sceneNode = rootSceneNode->addChildSceneNode( name );
                FB_ASSERT( sceneNode );

                sceneNode->attachObject( light );
                setSceneNode( sceneNode );
            }

            sceneManager->registerComponentUpdate( Thread::Task::Render, Thread::UpdateState::Transform,
                                                   this );
            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Light::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    if( auto light = getLight() )
                    {
                        if( auto sceneNode = getSceneNode() )
                        {
                            sceneNode->detachObject( light );
                        }

                        smgr->removeGraphicsObject( light );
                        setLight( nullptr );
                    }

                    if( auto sceneNode = getSceneNode() )
                    {
                        smgr->removeSceneNode( sceneNode );
                        setSceneNode( nullptr );
                    }
                }

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Light::updateDebugDraw()
    {
        auto &applicationManager = core::ApplicationManager::instance();
        auto &graphicsSystem = applicationManager->getGraphicsSystem();
        auto &debug = graphicsSystem->getDebug();

        if( auto actor = getActor() )
        {
            auto pos = actor->getPosition();
            debug->drawLine( lightHash, pos, pos + Vector3F::unitX(), 0xFFFFFF );
        }
    }

    void Light::updateFlags( u32 flags, u32 oldFlags )
    {
        if( auto actor = getActor() )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto rootNode = smgr->getRootSceneNode();

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
                {
                    auto visible = isEnabled() && actor->isEnabledInScene();

                    if( auto light = getLight() )
                    {
                        light->setVisible( visible );
                    }
                }
                else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                         BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    auto visible = isEnabled() && actor->isEnabledInScene();

                    if( auto light = getLight() )
                    {
                        light->setVisible( visible );
                    }
                }
            }
        }
    }

    auto Light::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();

        auto diffuseColour = getDiffuseColour();
        auto specularColour = getSpecularColour();

        properties->setProperty( "diffuseColour", diffuseColour );
        properties->setProperty( "specularColour", specularColour );
        properties->setProperty( "lightType", static_cast<u32>( m_lightType ) );

        return properties;
    }

    void Light::setProperties( SmartPtr<Properties> properties )
    {
        auto diffuseColour = getDiffuseColour();
        auto specularColour = getSpecularColour();

        properties->getPropertyValue( "diffuseColour", diffuseColour );
        properties->getPropertyValue( "specularColour", specularColour );

        u32 lightType = static_cast<u32>( m_lightType );
        properties->getPropertyValue( "lightType", lightType );

        m_diffuseColour = diffuseColour;
        m_specularColour = specularColour;

        if( auto light = getLight() )
        {
            light->setDiffuseColour( diffuseColour );
            light->setSpecularColour( specularColour );

            if( lightType < static_cast<u32>( render::ILight::LightTypes::Count ) )
            {
                if( lightType != static_cast<u32>( m_lightType ) )
                {
                    m_lightType = static_cast<render::ILight::LightTypes>( lightType );
                    light->setType( m_lightType );
                }
            }
        }
    }

    void Light::updateTransform()
    {
        if( auto actor = getActor() )
        {
            if( auto actorTransform = actor->getTransform() )
            {
                auto actorPosition = actorTransform->getPosition();
                auto actorOrientation = actorTransform->getOrientation();
                auto actorScale = actorTransform->getScale();

                if( auto sceneNode = getSceneNode() )
                {
                    sceneNode->setPosition( actorPosition );
                    sceneNode->setOrientation( actorOrientation );
                    sceneNode->setScale( actorScale );
                }
            }
        }
    }

    void Light::updateTransform( const Transform3<real_Num> &transform )
    {
        if( auto &sceneNode = getSceneNode() )
        {
            auto &p = transform.getPosition();
            auto &r = transform.getOrientation();
            auto &s = transform.getScale();

            sceneNode->setPosition( p );
            sceneNode->setOrientation( r );
            sceneNode->setScale( s );
        }
    }

    auto Light::getLightType() const -> render::ILight::LightTypes
    {
        return m_lightType;
    }

    void Light::setLightType( render::ILight::LightTypes lightType )
    {
        m_lightType = lightType;

        if( m_light )
        {
            m_light->setType( m_lightType );
        }
    }

    auto Light::getDiffuseColour() const -> ColourF
    {
        return m_diffuseColour;
    }

    void Light::setDiffuseColour( const ColourF &diffuseColour )
    {
        m_diffuseColour = diffuseColour;
    }

    auto Light::getSpecularColour() const -> ColourF
    {
        return m_specularColour;
    }

    void Light::setSpecularColour( const ColourF &specularColour )
    {
        m_specularColour = specularColour;
    }

    auto Light::getLight() const -> SmartPtr<render::ILight>
    {
        return m_light;
    }

    void Light::setLight( SmartPtr<render::ILight> light )
    {
        m_light = light;
    }

    void Light::setSceneNode( SmartPtr<render::ISceneNode> sceneNode )
    {
        m_sceneNode = sceneNode;
    }
}  // namespace fb::scene
