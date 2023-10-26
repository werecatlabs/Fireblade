#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementContainer.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreRoot.h>
#include <OgreResourceGroupManager.h>
#include <OgreGpuProgramParams.h>
#include <OgreHlmsPbsDatablock.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsPbsDatablock.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsManager.h>
#include <OgreTextureGpuManager.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb::render, COverlayElementContainer,
                                   COverlayElementOgre<IOverlayElementContainer> );

        COverlayElementContainer::COverlayElementContainer() :
            COverlayElementOgre<IOverlayElementContainer>()
        {
            createStateContext();
        }

        COverlayElementContainer::~COverlayElementContainer()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void COverlayElementContainer::load( SmartPtr<ISharedObject> data )
        {
            using namespace Ogre;

            try
            {
                setLoadingState( LoadingState::Loading );

                auto overlayMgr = Ogre::v1::OverlayManager::getSingletonPtr();
                FB_ASSERT( overlayMgr );

                auto name = getName();
                auto panel =
                    (Ogre::v1::OverlayContainer *)overlayMgr->createOverlayElement( "Panel", name );
                FB_ASSERT( panel );

                panel->setMetricsMode( Ogre::v1::GuiMetricsMode::GMM_PIXELS );
                panel->initialise();
                panel->updatePositionGeometry();
                panel->updateTextureGeometry();

                panel->setMetricsMode( Ogre::v1::GuiMetricsMode::GMM_PIXELS );
                panel->setLeft( 0 );
                panel->setTop( 0 );
                panel->setWidth( 100.0 );
                panel->setHeight( 100.0 );
                panel->show();

                setElement( panel );
                setContainerElement( panel );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayElementContainer::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( isLoaded() )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    if( m_materialStateListener )
                    {
                        if( m_material )
                        {
                            if( auto materialStateObject = m_material->getStateObject() )
                            {
                                materialStateObject->removeStateListener( m_materialStateListener );
                            }
                        }

                        m_materialStateListener->unload( nullptr );
                        m_materialStateListener = nullptr;
                    }

                    if( m_stateObject )
                    {
                        if( m_stateListener )
                        {
                            m_stateObject->removeStateListener( m_stateListener );
                        }

                        stateManager->removeStateObject( m_stateObject );

                        m_stateObject->unload( nullptr );
                        m_stateObject = nullptr;
                    }

                    if( m_stateListener )
                    {
                        m_stateListener->unload( nullptr );
                        m_stateListener = nullptr;
                    }

                    if( auto container = getElement() )
                    {
                        container->_setNullDatablock();

                        auto overlayMgr = Ogre::v1::OverlayManager::getSingletonPtr();
                        FB_ASSERT( overlayMgr );

                        container->_notifyParent( container->getParent(), nullptr );
                        overlayMgr->destroyOverlayElement( container );
                        setElement( nullptr );
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayElementContainer::setupMaterial( SmartPtr<IMaterial> material )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_material )
                    {
                        auto materialStateObject = m_material->getStateObject();
                        if( materialStateObject )
                        {
                            materialStateObject->removeStateListener( m_materialStateListener );
                        }
                    }

                    m_material = material;

                    if( m_material )
                    {
                        auto materialStateObject = m_material->getStateObject();
                        if( materialStateObject )
                        {
                            materialStateObject->addStateListener( m_materialStateListener );
                        }
                    }

                    if( m_material )
                    {
                        if( m_material->getLoadingState() == LoadingState::Loaded )
                        {
                            auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( m_material );
                            auto datablock = pMaterial->getHlmsDatablock();
                            if( datablock )
                            {
                                auto materialName = pMaterial->getDatablockName();

                                FB_ASSERT( !StringUtil::isNullOrEmpty( materialName ) );

                                m_container->setMaterialName( materialName );

                                //m_container->setDatablock(datablock);
                                m_container->updatePositionGeometry();
                                m_container->updateTextureGeometry();

                                m_container->setDatablock( datablock );
                            }
                        }
                        else
                        {
                            auto message = factoryManager->make_ptr<StateMessageMaterial>();
                            message->setMaterial( material );

                            m_stateObject->addMessage( stateTask, message );
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageMaterial>();
                    message->setMaterial( material );

                    m_stateObject->addMessage( stateTask, message );
                }
            }
            catch( Ogre::Exception &e )
            {
                auto errorMessage = String( e.getFullDescription().c_str() );
                FB_LOG_ERROR( errorMessage );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool COverlayElementContainer::isContainer() const
        {
            return true;
        }

        void COverlayElementContainer::addChild( SmartPtr<IOverlayElement> element )
        {
            try
            {
                FB_ASSERT( element );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    const auto &childLoadingState = element->getLoadingState();

                    if( childLoadingState == LoadingState::Loaded )
                    {
                        element->setParent( this );

                        Ogre::v1::OverlayElement *ogreElement;
                        element->_getObject( (void **)&ogreElement );

                        FB_ASSERT( m_container );
                        FB_ASSERT( ogreElement );

                        if( m_container )
                        {
                            if( ogreElement )
                            {
                                m_container->addChild( ogreElement );
                            }
                        }

                        COverlayElementOgre<IOverlayElementContainer>::addChild( element );
                    }
                    else
                    {
                        auto message = factoryManager->make_ptr<StateMessageObject>();
                        message->setType( STATE_MESSAGE_ADDCHILD );
                        message->setObject( element );

                        m_stateObject->addMessage( stateTask, message );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_ADDCHILD );
                    message->setObject( element );

                    m_stateObject->addMessage( stateTask, message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayElementContainer::removeChild( SmartPtr<IOverlayElement> element )
        {
            try
            {
                FB_ASSERT( element );

                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto factoryManager = applicationManager->getFactoryManager();

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( element )
                    {
                        element->setParent( nullptr );

                        Ogre::v1::OverlayElement *ogreElement;
                        element->_getObject( (void **)&ogreElement );

                        if( m_container )
                        {
                            if( ogreElement )
                            {
                                m_container->removeChild( ogreElement->getName() );
                            }
                        }

                        COverlayElementOgre<IOverlayElementContainer>::removeChild( element );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_REMOVECHILD );
                    message->setObject( element );

                    m_stateObject->addMessage( stateTask, message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayElementContainer::createStateContext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            m_stateObject = stateManager->addStateObject();
            m_stateObject->setOwner( this );

            auto listener = factoryManager->make_ptr<StateListener>();
            listener->setOwner( this );
            m_stateListener = listener;
            m_stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<OverlayContainerState>();
            m_stateObject->setState( state );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );

            auto materialStateListener = factoryManager->make_ptr<MaterialStateListener>();
            materialStateListener->setOwner( this );
            m_materialStateListener = materialStateListener;
        }

        void COverlayElementContainer::materialLoaded( SmartPtr<IMaterial> material )
        {
            if( material )
            {
                setMaterial( material );
            }
        }

        SmartPtr<Properties> COverlayElementContainer::getProperties() const
        {
            auto properties = COverlayElementOgre<IOverlayElementContainer>::getProperties();
            properties->setProperty( "materialName", m_materialName );

            if( auto element = getElement() )
            {
                auto position = Vector2F( element->getLeft(), element->getTop() );
                properties->setProperty( "position", position );

                auto size = Vector2F( element->getWidth(), element->getHeight() );
                properties->setProperty( "size", size );
            }

            properties->setProperty( "Type", "OverlayElementContainer" );
            properties->setProperty( "MetricsMode",
                                     Ogre::StringConverter::toString( m_container->getMetricsMode() ) );
            properties->setProperty(
                "HorizontalAlignment",
                Ogre::StringConverter::toString( m_container->getHorizontalAlignment() ) );
            properties->setProperty( "VerticalAlignment", Ogre::StringConverter::toString(
                                                              m_container->getVerticalAlignment() ) );
            properties->setProperty( "Left", Ogre::StringConverter::toString( m_container->getLeft() ) );
            properties->setProperty( "Top", Ogre::StringConverter::toString( m_container->getTop() ) );
            properties->setProperty( "Width",
                                     Ogre::StringConverter::toString( m_container->getWidth() ) );
            properties->setProperty( "Height",
                                     Ogre::StringConverter::toString( m_container->getHeight() ) );
            properties->setProperty( "Visible",
                                     Ogre::StringConverter::toString( m_container->isVisible() ) );
            properties->setProperty( "MaterialName", m_container->getMaterialName() );
            properties->setProperty( "Colour",
                                     Ogre::StringConverter::toString( m_container->getColour() ) );
            properties->setProperty( "Caption", m_container->getCaption() );

            return properties;
        }

        void COverlayElementContainer::setProperties( SmartPtr<Properties> properties )
        {
            COverlayElementOgre<IOverlayElementContainer>::setProperties( properties );
        }

        Array<SmartPtr<ISharedObject>> COverlayElementContainer::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;
            objects.reserve( 6 );

            if( m_material )
            {
                objects.push_back( m_material );
            }

            if( m_stateObject )
            {
                objects.push_back( m_stateObject );
            }

            if( m_stateListener )
            {
                objects.push_back( m_stateListener );
            }

            if( m_materialStateListener )
            {
                objects.push_back( m_materialStateListener );
            }

            return objects;
        }

        bool COverlayElementContainer::isValid() const
        {
            const auto &state = getLoadingState();
            switch( state )
            {
            case LoadingState::Unloaded:
            {
                if( m_stateObject || m_stateListener || m_materialStateListener )
                {
                    return false;
                }

                return m_container == nullptr;
            }
            break;
            case LoadingState::Loading:
            {
                if( m_stateObject || m_stateListener || m_materialStateListener )
                {
                    return false;
                }

                return m_container == nullptr;
            }
            break;
            case LoadingState::Loaded:
            {
                if( m_stateObject && m_stateListener && m_materialStateListener )
                {
                    if( m_stateObject->isValid() && m_stateListener->isValid() &&
                        m_materialStateListener->isValid() && m_container != nullptr )
                    {
                        return true;
                    }

                    return false;
                }
            }
            break;

            case LoadingState::Unloading:
            {
                return m_container == nullptr;
            }
            break;
            default:
            {
            }
            };

            return COverlayElementOgre<IOverlayElementContainer>::isValid();
        }

        Ogre::v1::OverlayContainer *COverlayElementContainer::getContainerElement() const
        {
            return m_container;
        }

        void COverlayElementContainer::setContainerElement( Ogre::v1::OverlayContainer *container )
        {
            m_container = container;
        }

        void COverlayElementContainer::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            StateListenerOgre::handleStateChanged( state );
        }

        void COverlayElementContainer::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            StateListenerOgre::handleStateChanged( message );

            if( auto owner = getOwner() )
            {
                // auto applicationManager = core::IApplicationManager::instance();
                // auto graphicsSystem = applicationManager->getGraphicsSystem();
                // auto factoryManager = applicationManager->getFactoryManager();

                if( message->isExactly<StateMessageMaterial>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageMaterial>( message );
                    FB_ASSERT( objectMessage );

                    auto material = objectMessage->getMaterial();

                    owner->setMaterial( material );
                }
                else if( message->isExactly<StateMessageMaterialName>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageMaterialName>( message );
                    FB_ASSERT( objectMessage );

                    //owner->setMaterialName( objectMessage->getMaterialName() );
                }
                else if( message->isExactly<StateMessageVisible>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                    FB_ASSERT( objectMessage );

                    owner->setVisible( objectMessage->isVisible() );
                }
                else if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    FB_ASSERT( objectMessage );

                    auto messageType = objectMessage->getType();
                    auto object = objectMessage->getObject();

                    if( messageType == STATE_MESSAGE_ADDCHILD )
                    {
                        owner->addChild( object );
                    }
                    else if( messageType == STATE_MESSAGE_REMOVECHILD )
                    {
                        owner->removeChild( object );
                    }
                }
                else if( message->isExactly<StateMessageUIntValue>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );
                    FB_ASSERT( objectMessage );

                    auto messageType = objectMessage->getType();
                    auto messageValue = objectMessage->getValue();

                    if( messageType == STATE_MESSAGE_METRICSMODE )
                    {
                        owner->setMetricsMode( (u8)messageValue );
                    }
                    else if( messageType == STATE_MESSAGE_ALIGN_HORIZONTAL )
                    {
                        owner->setHorizontalAlignment( (u8)messageValue );
                    }
                    else if( messageType == STATE_MESSAGE_ALIGN_VERTICAL )
                    {
                        owner->setVerticalAlignment( (u8)messageValue );
                    }
                }
                else if( message->isExactly<StateMessageFloatValue>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageFloatValue>( message );
                    FB_ASSERT( objectMessage );

                    auto messageType = objectMessage->getType();
                    auto messageValue = objectMessage->getValue();

                    if( messageType == StateMessageFloatValue::WIDTH_HASH )
                    {
                        //owner->setWidth( messageValue );
                    }
                    else if( messageType == StateMessageFloatValue::HEIGHT_HASH )
                    {
                        //owner->setHeight( messageValue );
                    }
                    else if( messageType == StateMessageFloatValue::TOP_HASH )
                    {
                        //owner->setTop( messageValue );
                    }
                    else if( messageType == StateMessageFloatValue::LEFT_HASH )
                    {
                        //owner->setLeft( messageValue );
                    }
                }
            }
        }

        void COverlayElementContainer::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        COverlayElementContainer::MaterialStateListener::MaterialStateListener(
            COverlayElementContainer *owner )
        {
            m_owner = owner;
        }

        COverlayElementContainer::MaterialStateListener::~MaterialStateListener()
        {
        }

        void COverlayElementContainer::MaterialStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                if( auto stateContent = owner->getStateObject() )
                {
                    stateContent->setDirty( true );
                }
            }
        }

        void COverlayElementContainer::MaterialStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( owner->isLoaded() )
                {
                    if( message->isExactly<StateMessageLoad>() )
                    {
                        auto stateMessageLoad = fb::static_pointer_cast<StateMessageLoad>( message );
                        auto eventType = stateMessageLoad->getType();

                        if( eventType == StateMessageLoad::LOADED_HASH )
                        {
                            owner->materialLoaded( stateMessageLoad->getObject() );
                        }
                    }
                }
            }
        }

        void COverlayElementContainer::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<COverlayElementContainer> COverlayElementContainer::MaterialStateListener::getOwner()
            const
        {
            return m_owner;
        }

        void COverlayElementContainer::MaterialStateListener::setOwner(
            SmartPtr<COverlayElementContainer> val )
        {
            m_owner = val;
        }

    }  // end namespace render
}  // end namespace fb
