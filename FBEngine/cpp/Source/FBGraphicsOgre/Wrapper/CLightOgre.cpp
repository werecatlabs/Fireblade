#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CLightOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBGraphicsOgre/Ogre/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <OgreLight.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CLightOgre, CGraphicsObjectOgre<ILight> );

        CLightOgre::CLightOgre()
        {
            setupStateObject();
        }

        CLightOgre::~CLightOgre()
        {
            unload( nullptr );
        }

        void CLightOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                FB_ASSERT( !getOwner() );

                auto name = String();

                if( auto handle = getHandle() )
                {
                    name = handle->getName();
                }

                Ogre::SceneManager *smgr = nullptr;

                if( auto creator = getCreator() )
                {
                    creator->_getObject( (void **)&smgr );
                }

                if( smgr )
                {
                    auto ogreLight = smgr->createLight( name );
                    m_light = ogreLight;

                    m_dummyNode = smgr->createSceneNode();
                    m_dummyNode->attachObject( ogreLight );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( Ogre::Exception &e )
            {
                auto message = e.getFullDescription();
                FB_LOG_ERROR( message );
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CLightOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto owner = fb::static_pointer_cast<CSceneNodeOgre>( getOwner() );
                    if( owner )
                    {
                        owner->detachObjectPtr( this );
                        setOwner( nullptr );
                    }

                    if( auto smgr = getCreator() )
                    {
                        Ogre::SceneManager *ogreSmgr = nullptr;
                        smgr->_getObject( (void **)&ogreSmgr );

                        if( m_dummyNode )
                        {
                            m_dummyNode->detachAllObjects();
                            ogreSmgr->destroySceneNode( m_dummyNode );
                            m_dummyNode = nullptr;
                        }

                        if( auto light = getLight() )
                        {
                            ogreSmgr->destroyLight( light );
                            setLight( nullptr );
                        }
                    }

                    CGraphicsObjectOgre<ILight>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( Ogre::Exception &e )
            {
                auto message = e.getFullDescription();
                FB_LOG_ERROR( message );
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CLightOgre::setRenderQueueGroup( u8 renderQueue )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    lightState->setRenderQueueGroup( renderQueue );
                }
            }
        }

        void CLightOgre::setVisibilityFlags( u32 flags )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    lightState->setVisibilityMask( flags );
                }
            }
        }

        u32 CLightOgre::getVisibilityFlags() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    return lightState->getVisibilityMask();
                }
            }

            return 0;
        }

        SmartPtr<IGraphicsObject> CLightOgre::clone( const String &name ) const
        {
            return nullptr;
        }

        void CLightOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_light;
        }

        void CLightOgre::setType( LightTypes type )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    lightState->setLightType( (hash32)type );
                }
            }
        }

        ILight::LightTypes CLightOgre::getType() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    auto eTypes = (ILight::LightTypes)lightState->getLightType();
                    return eTypes;
                }
            }

            return LightTypes::LT_POINT;
        }

        void CLightOgre::setDiffuseColour( const ColourF &colour )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    lightState->setDiffuseColour( colour );
                }
            }
        }

        ColourF CLightOgre::getDiffuseColour() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    return lightState->getDiffuseColour();
                }
            }

            return ColourF::White;
        }

        void CLightOgre::setSpecularColour( const ColourF &colour )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    lightState->setSpecularColour( colour );
                }
            }
        }

        ColourF CLightOgre::getSpecularColour() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto lightState = stateObject->getStateByType<LightState>() )
                {
                    return lightState->getSpecularColour();
                }
            }

            return ColourF::White;
        }

        void CLightOgre::setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic )
        {
            m_light->setAttenuation( range, constant, linear, quadratic );
        }

        f32 CLightOgre::getAttenuationRange() const
        {
            return 0.0f;
        }

        f32 CLightOgre::getAttenuationConstant() const
        {
            return 0.0f;
        }

        f32 CLightOgre::getAttenuationLinear() const
        {
            return 0.0f;
        }

        f32 CLightOgre::getAttenuationQuadric() const
        {
            return 0.0f;
        }

        void CLightOgre::setDirection( const Vector3F &vec )
        {
            if( m_light )
            {
                Ogre::Vector3 dir( vec.X(), vec.Y(), vec.Z() );
                //m_light->setDirection(dir);
            }
        }

        Vector3F CLightOgre::getDirection() const
        {
            auto dir = Ogre::Vector3::UNIT_Y;  // m_light->getDirection();
            return Vector3F( dir.x, dir.y, dir.z );
        }

        Ogre::Light *CLightOgre::getLight() const
        {
            return m_light;
        }

        void CLightOgre::setLight( Ogre::Light *light )
        {
            m_light = light;
        }

        void CLightOgre::setupStateObject()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                CGraphicsObjectOgre<ILight>::setupStateObject();

                auto stateObject = getStateObject();

                auto lightState = factoryManager->make_ptr<LightState>();
                lightState->setOwner( this );

                auto stateTask = graphicsSystem->getStateTask();
                lightState->setTaskId( stateTask );

                stateObject->setState( lightState );
                setGraphicsObjectState( lightState );

                auto stateListener = fb::make_ptr<CLightStateListener>();
                stateListener->setOwner( this );

                m_stateListener = stateListener;
                stateObject->addStateListener( m_stateListener );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CLightOgre::attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        void CLightOgre::detachFromParent( SmartPtr<ISceneNode> parent )
        {
            if( !parent )
            {
                if( m_dummyNode )
                {
                    if( m_light->isAttached() && m_light->getParentSceneNode() == m_dummyNode )
                    {
                        m_dummyNode->detachObject( m_light );
                    }
                }
            }
        }

        void CLightOgre::CLightStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

                    if( type == STATE_MESSAGE_DIRECTION )
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
                        m_owner->setDiffuseColour(
                            ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
                    }
                }
            }
        }

        void CLightOgre::CLightStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            try
            {
                if( auto owner = getOwner() )
                {
                    auto lightState = fb::static_pointer_cast<LightState>( state );
                    if( lightState )
                    {
                        auto visible = lightState->isVisible();

                        auto d = lightState->getDiffuseColour();
                        FB_ASSERT( d.isValid() );
                        auto diffuse = OgreUtil::convertToOgre( d );

                        auto s = lightState->getSpecularColour();
                        FB_ASSERT( s.isValid() );
                        auto specular = OgreUtil::convertToOgre( s );

                        auto lightType = lightState->getLightType();
                        auto renderQueue = lightState->getRenderQueueGroup();

                        if( auto light = owner->getLight() )
                        {
                            light->setVisible( visible );
                            light->setRenderQueueGroup( renderQueue );

                            light->setDiffuseColour( diffuse );
                            light->setSpecularColour( specular );

                            if( lightType < 3 )
                            {
                                light->setType( (Ogre::Light::LightTypes)lightType );
                            }

                            lightState->setDirty( false );
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CLightOgre::CLightStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CLightOgre *CLightOgre::CLightStateListener::getOwner() const
        {
            return m_owner;
        }

        void CLightOgre::CLightStateListener::setOwner( CLightOgre *owner )
        {
            m_owner = owner;
        }
    }  // end namespace render
}  // end namespace fb
