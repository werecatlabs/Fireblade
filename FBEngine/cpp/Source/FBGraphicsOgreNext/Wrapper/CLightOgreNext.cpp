#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CLightOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h>
#include <FBCore/FBCore.h>
#include <OgreLight.h>
#include <Ogre.h>

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED( fb, CLightOgreNext, Light );

    u32 CLightOgreNext::m_nameExt = 0;

    CLightOgreNext::CLightOgreNext()
    {
        static const auto Str = String( "Light" );
        auto name = Str + StringUtil::toString( m_nameExt++ );
        getHandle()->setName( name );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto stateContext = stateManager->addStateObject();

        auto stateListener = factoryManager->make_ptr<CLightStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<LightState>();
        // state->setSceneNode(this);
        stateContext->addState( state );
        stateContext->setOwner( this );

        setStateContext( stateContext );
        setGraphicsObjectState( state );

        state->setTaskId( Thread::Task::Render );
    }

    CLightOgreNext::CLightOgreNext( SmartPtr<IGraphicsScene> creator )
    {
        setCreator( creator );

        static const auto Str = String( "Light" );
        auto name = Str + StringUtil::toString( m_nameExt++ );
        getHandle()->setName( name );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto stateContext = stateManager->addStateObject();

        auto sceneNodeStateListener = factoryManager->make_ptr<CLightStateListener>();
        sceneNodeStateListener->setOwner( this );
        m_stateListener = sceneNodeStateListener;
        stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<LightState>();
        // sceneNodeState->setSceneNode(this);
        stateContext->addState( state );
        stateContext->setOwner( this );

        setStateContext( stateContext );
        setGraphicsObjectState( state );

        state->setTaskId( Thread::Task::Render );
    }

    CLightOgreNext::~CLightOgreNext()
    {
        unload( nullptr );
    }

    void CLightOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Ogre::SceneManager *smgr = nullptr;
            m_creator->_getObject( reinterpret_cast<void **>( &smgr ) );

            // m_light = smgr->createLight();
            // m_light->setType( Ogre::Light::LT_DIRECTIONAL );
            // m_light->setDiffuseColour( Ogre::ColourValue::White * 1.0f );
            // m_light->setSpecularColour( Ogre::ColourValue::White );
            ////m_light->setStatic( true );
            // m_light->setAttenuation( 3000.0, 0.7, 1.0, 1.0 );
            // m_light->setAttenuationBasedOnRadius( 3000.0f, 0.8f );

            Ogre::SceneNode *rootNode = smgr->getRootSceneNode();

            Ogre::Light *light = smgr->createLight();
            light->setCastShadows( true );
            light->setPowerScale( 1.0f );
            light->setType( Ogre::Light::LT_DIRECTIONAL );
            m_light = light;

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CLightOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto owner = getOwner();
            if( owner )
            {
                owner->detachObject( this );
                setOwner( nullptr );
            }

            if( m_light )
            {
                Ogre::SceneManager *smgr = nullptr;
                m_creator->_getObject( reinterpret_cast<void **>( &smgr ) );

                smgr->destroyLight( m_light );

                m_light = nullptr;
            }

            Light::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CLightOgreNext::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
    {
        auto light = fb::make_ptr<CLightOgreNext>();
        return light;
    }

    void CLightOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_light;
    }

    auto CLightOgreNext::getLight() const -> Ogre::Light *
    {
        return m_light;
    }

    CLightOgreNext::CLightStateListener::CLightStateListener() = default;

    CLightOgreNext::CLightStateListener::CLightStateListener( CLightOgreNext *light ) : m_owner( light )
    {
    }

    CLightOgreNext::CLightStateListener::~CLightStateListener() = default;

    void CLightOgreNext::CLightStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto renderTask = graphicsSystem->getRenderTask();
        auto currentTaskId = Thread::getCurrentTask();
        FB_ASSERT( currentTaskId == renderTask );

        if( currentTaskId == renderTask )
        {
            if( message->isExactly<StateMessageVisible>() )
            {
                auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                FB_ASSERT( visibleMessage );

                auto visible = visibleMessage->isVisible();
                m_owner->setVisible( visible );
            }
            else if( message->isExactly<StateMessageUIntValue>() )
            {
                auto valueMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );
                auto type = valueMessage->getType();
                auto value = valueMessage->getValue();

                if( type == VISIBILITY_MASK_HASH )
                {
                    m_owner->setVisibilityFlags( value );
                }
            }
            else if( message->isExactly<StateMessageIntValue>() )
            {
                auto valueMessage = fb::static_pointer_cast<StateMessageIntValue>( message );
                auto type = valueMessage->getType();
                auto value = valueMessage->getValue();

                if( type == LIGHT_TYPE_HASH )
                {
                    m_owner->setType( static_cast<ILight::LightTypes>( value ) );
                }
            }
            else if( message->isExactly<StateMessageVector3>() )
            {
                auto valueMessage = fb::static_pointer_cast<StateMessageVector3>( message );
                auto type = valueMessage->getType();
                auto value = valueMessage->getValue();

                if( type == GraphicsOgreNextTypes::STATE_MESSAGE_DIRECTION )
                {
                    m_owner->setDirection( value );
                }
            }
            else if( message->isExactly<StateMessageVector4>() )
            {
                auto valueMessage = fb::static_pointer_cast<StateMessageVector4>( message );
                auto type = valueMessage->getType();
                auto value = valueMessage->getValue();

                if( type == DIFFUSE_COLOUR_HASH )
                {
                    m_owner->setDiffuseColour( ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
                }
            }
        }
    }

    void CLightOgreNext::CLightStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto lightState = fb::static_pointer_cast<LightState>( state );
        auto visible = lightState->isVisible();

        auto iLightType = lightState->getLightType();
        auto lightType = static_cast<Ogre::Light::LightTypes>( iLightType );

        auto visibilityMask = lightState->getVisibilityMask();

        auto diffuse = lightState->getDiffuseColour();
        auto specular = lightState->getSpecularColour();

        auto castShadows = lightState->getCastShadows();
        auto renderQueue = lightState->getRenderQueueGroup();

        auto attenuation = lightState->getAttenuation();
        auto range = lightState->getRange();
        auto constant = lightState->getConstant();
        auto linear = lightState->getLinear();
        auto quadratic = lightState->getQuadratic();

        auto direction = lightState->getDirection();

        if( auto owner = getOwner() )
        {
            if( auto light = owner->getLight() )
            {
                if( light->isAttached() )
                {
                    light->setType( lightType );
                    light->setVisibilityFlags( visibilityMask );
                    light->setAttenuation( range, constant, linear, quadratic );

                    light->setCastShadows( castShadows );
                    light->setRenderQueueGroup( renderQueue );

                    light->setDiffuseColour(
                        Ogre::ColourValue( diffuse.r, diffuse.g, diffuse.b, diffuse.a ) );
                    light->setSpecularColour(
                        Ogre::ColourValue( specular.r, specular.g, specular.b, specular.a ) );
                    light->setDirection( Ogre::Vector3( direction.X(), direction.Y(), direction.Z() ) );
                    light->setVisible( visible );

                    state->setDirty( false );
                }
            }
        }
    }

    void CLightOgreNext::CLightStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    void CLightOgreNext::CLightStateListener::setOwner( CLightOgreNext *light )
    {
        m_owner = light;
    }

    auto CLightOgreNext::CLightStateListener::getOwner() const -> CLightOgreNext *
    {
        return m_owner;
    }

}  // namespace fb::render
