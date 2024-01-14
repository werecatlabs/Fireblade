#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CParticleSystem.h"
#include "FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h"

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include "ParticleAffectors/ParticleUniverseColourAffector.h"
#endif

#include "FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h"
#include <FBCore/Interface/Particle/IParticleNode.h>

namespace fb
{

#if FB_OGRE_USE_PARTICLE_UNIVERSE
    const hash32 PUParticleSystem::STATE_MESSAGE_RENDER_QUEUE_GROUP =
        StringUtil::getHash( "render_queue_group" );

    //--------------------------------------------
    PUParticleSystem::PUParticleSystem( SmartPtr<render::IGraphicsScene> creator ) :
        m_creator( creator ),
        m_particleSystem( nullptr )
    {
        auto engine = core::ApplicationManager::instance();
        SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();

        m_frameListener = SmartPtr<IFrameListener>( new ParticleSystemFrameListener( this ) );

        SmartPtr<IStateContext> stateContext = engine->getPlatformManager()->createStateObject();
        setStateContext( stateContext );

        stateContext->addStateListener(
            SmartPtr<IStateListener>( new ParticleSystemStateListener( this ) ), true );
    }

    //--------------------------------------------
    PUParticleSystem::~PUParticleSystem()
    {
        //

        // auto engine = core::ApplicationManager::instance();
        // SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();

        // SmartPtr<CSceneManagerOgre> smgr = m_creator;

        // m_particleSystem->detachFromParent();

        // ParticleUniverse::ParticleSystemManager* particleSystemManager =
        // ParticleUniverse::ParticleSystemManager::getSingletonPtr();
        // particleSystemManager->destroyParticleSystem(m_particleSystem, smgr->getSceneManager());

        // smgr->_removeGraphicsObject(this);

        // if (graphicsSystem)
        //{
        //	graphicsSystem->removeFrameListener(m_frameListener);
        // }
    }

    //--------------------------------------------
    void PUParticleSystem::handleMessage( const SmartPtr<IMessage> &message )
    {
        u32 msgType = message->getTypeId();
        switch( msgType )
        {
        case PSMT_UPDATE_AFFECTOR_COLOURS:
        {
            //

            // ColourAffectorMsgPtr colourAffectorMsg = message;

            // const String& techinqueName = colourAffectorMsg->getTechniqueName();
            // const String& affectorName = colourAffectorMsg->getAffectorName();
            // const Array<TimeColourPair>& timeColourPairs =
            // colourAffectorMsg->getTimeColourPairs();

            // ParticleUniverse::ParticleTechnique* techinque =
            // m_particleSystem->getTechnique(techinqueName.c_str()); ParticleUniverse::ParticleAffector*
            // affector = techinque->getAffector(affectorName.c_str());

            ////todo add assert to check type
            ////if(affector->getAffectorType().e)
            //{
            //	ParticleUniverse::ColourAffector* colourAffector =
            // static_cast<ParticleUniverse::ColourAffector*>(affector);
            // colourAffector->clearColourMap();

            //	for(u32 i=0; i<timeColourPairs.size(); ++i)
            //	{
            //		const TimeColourPair& timeColourPair = timeColourPairs[i];
            //		const ColourF& colour = timeColourPair.second;
            //		colourAffector->addColour(timeColourPair.first, Ogre::ColourValue(colour.r, colour.g,
            // colour.b, colour.a));
            //	}
            //}
        }
        break;
        case PSMT_UPDATE_EMITTER_COLOUR:
        {
            // EmitterColourMsgPtr emitterColourMsg = message;

            // const String& techinqueName = emitterColourMsg->getTechniqueName();
            // const String& emitterName = emitterColourMsg->getEmitterName();
            // const ColourF& colour = emitterColourMsg->getColour();

            //{
            //
            //	ParticleUniverse::ParticleTechnique* techinque =
            // m_particleSystem->getTechnique(techinqueName.c_str()); 	if(techinque)
            //	{
            //		ParticleUniverse::ParticleEmitter* emitter =
            // techinque->getEmitter(emitterName.c_str()); 		if(emitter)
            //		{
            //			emitter->setParticleColour(Ogre::ColourValue(colour.r, colour.g, colour.b,
            // colour.a));
            //		}
            //		else
            //		{
            //			String message = String("CParticleSystem::handleMessage emitter not found. Name:
            //")
            //+ emitterName; 			LOG_MESSAGE("CParticleSystem", message.c_str());
            //		}
            //	}
            //}
        }
        break;
        case PSMT_EMITTER_POSITION:
        {
            // EmitterPositionMsgPtr emitterPositionMsg = message;
            // if(emitterPositionMsg->getApplyAll())
            //{
            //	const String& emitterName = emitterPositionMsg->getEmitterName();
            //	const Vector3F& position = emitterPositionMsg->getPosition();

            //	u32 numTechniques = m_particleSystem->getNumTechniques();
            //	for(u32 i=0; i<numTechniques; ++i)
            //	{
            //		ParticleUniverse::ParticleTechnique* techinque = m_particleSystem->getTechnique(i);
            //		//techinque->setPosition(Ogre::Vector3(position));
            //		/*ParticleUniverse::ParticleEmitter* emitter =
            // techinque->getEmitter(emitterName.c_str()); 		if(emitter)
            //		{
            //		emitter->setPosition(Ogre::Vector3(position));
            //		}
            //		else
            //		{
            //		String message = String("CParticleSystem::handleMessage emitter not found. Name: ") +
            // emitterName; 		LOG_MESSAGE("CParticleSystem", message.c_str());
            //		}*/
            //	}
            //}
            // else
            //{

            //}
        }
        break;
        case PSMT_EMITTER_DIRECTION:
        {
            // ComponentDirectionMsgPtr emitterDirMsg = message;
            // if(emitterDirMsg->getApplyAll())
            //{
            //	const String& emitterName = emitterDirMsg->getEmitterName();
            //	const Vector3F& direction = emitterDirMsg->getDirection();

            //	u32 numTechniques = m_particleSystem->getNumTechniques();
            //	for(u32 i=0; i<numTechniques; ++i)
            //	{
            //		ParticleUniverse::ParticleTechnique* techinque = m_particleSystem->getTechnique(i);
            //
            //		ParticleUniverse::ParticleEmitter* emitter =
            // techinque->getEmitter(emitterName.c_str()); 		if(emitter)
            //		{
            //			emitter->setParticleDirection(Ogre::Vector3(direction));
            //		}
            //	}
            //}
            // else
            //{
            //	const String& techniqueName = emitterDirMsg->getTechniqueName();
            //	const String& emitterName = emitterDirMsg->getEmitterName();
            //	const Vector3F& direction = emitterDirMsg->getDirection();

            //	u32 numTechniques = m_particleSystem->getNumTechniques();
            //	for(u32 i=0; i<numTechniques; ++i)
            //	{
            //		ParticleUniverse::ParticleTechnique* techinque =
            // m_particleSystem->getTechnique(techniqueName.c_str()); 		if(techinque)
            //		{
            //			ParticleUniverse::ParticleEmitter* emitter =
            // techinque->getEmitter(emitterName.c_str()); 			if(emitter)
            //			{
            //				emitter->setParticleDirection(Ogre::Vector3(direction));
            //			}
            //		}
            //	}
            //}
        }
        break;
        default:
        {
        }
        };
    }

    //--------------------------------------------
    void PUParticleSystem::initialise( ParticleUniverse::ParticleSystem *particleSystem )
    {
        // m_particleSystem = particleSystem;

        // m_templateName = m_particleSystem->getTemplateName().c_str();

        // u32 numTechniques = m_particleSystem->getNumTechniques();
        // for(u32 i=0; i<numTechniques; ++i)
        //{
        //	ParticleUniverse::ParticleTechnique* puTech = m_particleSystem->getTechnique(i);
        //	if(puTech)
        //	{
        //		SmartPtr<IParticleTechnique> techPtr(new PUParticleTechnique(puTech));
        //		m_techniques[StringUtil::getHash(puTech->getName().c_str())] = techPtr;
        //	}
        // }
    }

    //--------------------------------------------
    void PUParticleSystem::update( const s32 &task, const time_interval &t, const time_interval &dt )
    {
        load();

        if( m_particleSystem )
            m_particleSystem->_update( dt );
    }

    //--------------------------------------------
    void PUParticleSystem::setName( const String &name )
    {
        m_name = name;
    }

    //--------------------------------------------
    String PUParticleSystem::getName() const
    {
        return m_name;
    }

    //--------------------------------------------
    u32 PUParticleSystem::getId() const
    {
        return 0;
    }

    //--------------------------------------------
    void PUParticleSystem::setMaterialName( const String &materialName, s32 index )
    {
    }

    //--------------------------------------------
    String PUParticleSystem::getMaterialName( s32 index ) const
    {
        return StringUtil::EmptyString;
    }

    //--------------------------------------------
    void PUParticleSystem::setCastShadows( bool castShadows )
    {
    }

    //--------------------------------------------
    bool PUParticleSystem::getCastShadows() const
    {
        return false;
    }

    //--------------------------------------------
    void PUParticleSystem::setRecieveShadows( bool recieveShadows )
    {
    }

    //--------------------------------------------
    bool PUParticleSystem::getRecieveShadows() const
    {
        return false;
    }

    //--------------------------------------------
    void PUParticleSystem::setVisible( bool isVisible )
    {
        m_visible = isVisible ? TRUE : FALSE;

        if( Thread::getCurrentTask() != Thread::Task::Render )
        {
            StateMessageVisiblePtr stateMessageVisible( new StateMessageVisible );
            stateMessageVisible->setVisible( isVisible );
            stateMessageVisible->setCascade( true );
            m_stateContext->addMessage( Thread::Task::Render, stateMessageVisible );
        }
        else
        {
            if( m_particleSystem )
                m_particleSystem->setVisible( isVisible );
        }
    }

    //--------------------------------------------
    bool PUParticleSystem::isVisible() const
    {
        return m_visible ? true : false;
    }

    //--------------------------------------------
    void PUParticleSystem::setRenderQueueGroup( u8 renderQueue )
    {
        m_renderQueueGroup = renderQueue;

        if( Thread::getCurrentTask() != Thread::Task::Render )
        {
            StateMessageUIntValuePtr stateMessage( new StateMessageUIntValue );
            stateMessage->setType( STATE_MESSAGE_RENDER_QUEUE_GROUP );
            stateMessage->setValue( renderQueue );
            m_stateContext->addMessage( Thread::Task::Render, stateMessage );
        }
        else
        {
            if( m_particleSystem )
                m_particleSystem->setRenderQueueGroup( renderQueue );
        }
    }

    //--------------------------------------------
    void PUParticleSystem::setVisibilityFlags( u32 flags )
    {
        if( Thread::getCurrentTask() == Thread::Task::Render )
        {
            if( m_particleSystem )
                m_particleSystem->setVisibilityFlags( flags );
        }
        else
        {
            StateMessageTypePtr message( new StateMessageType );
            FB_ASSERT( message );

            static const hash32 VISIBILITY_FLAGS_HASH = StringUtil::getHash( "visibilityFlags" );

            message->setType( VISIBILITY_FLAGS_HASH );
            message->setTypeValue( flags );
            m_stateContext->addMessage( Thread::Task::Render, message );
        }
    }

    //--------------------------------------------
    u32 PUParticleSystem::getVisibilityFlags() const
    {
        if( m_particleSystem )
        {
            return m_particleSystem->getVisibilityFlags();
        }

        return 0;
    }

    //--------------------------------------------
    SmartPtr<IGraphicsObject> PUParticleSystem::clone( const String &name ) const
    {
        return nullptr;
    }

    //--------------------------------------------
    void PUParticleSystem::_getObject( void **ppObject ) const
    {
        *ppObject = m_particleSystem;
    }

    //--------------------------------------------
    void PUParticleSystem::setPropertyValue( const String &name, const String &value )
    {
    }

    //--------------------------------------------
    bool PUParticleSystem::getPropertyValue( const String &name, String &value )
    {
        return false;
    }

    //--------------------------------------------
    void PUParticleSystem::setProperties( const Properties &propertyGroup )
    {
        /*String value;
        propertyGroup.GetPropertyValue("BaseType", value);
        if(value==("Affector"))
        {
        propertyGroup.GetPropertyValue("TechinqueName", value);
        ParticleUniverse::ParticleTechnique* techinque = m_particleSystem->getTechnique(value.c_str());

        propertyGroup.GetPropertyValue("AffectorName", value);
        ParticleUniverse::ParticleAffector* affector = techinque->getAffector(value.c_str());

        propertyGroup.GetPropertyValue("SubType", value);
        if(value==("Colour") || value==("Color"))
        {
        ParticleUniverse::ColourAffector* colourAffector =
        static_cast<ParticleUniverse::ColourAffector*>(affector); colourAffector->clearColourMap();
        }
        }*/
    }

    //--------------------------------------------
    bool PUParticleSystem::getProperties( Properties &propertyGroup, u32 flags ) const
    {
        return false;
    }

    //--------------------------------------------
    void PUParticleSystem::reload()
    {
        ParticleUniverse::ParticleSystemManager *particleSystemManager =
            ParticleUniverse::ParticleSystemManager::getSingletonPtr();
        // const Ogre::String& templateName = m_particleSystem->getTemplateName();
        ParticleUniverse::ParticleSystem *ps =
            particleSystemManager->getParticleSystemTemplate( m_templateName.c_str() );
        if( ps )
        {
            *m_particleSystem = *ps;
            m_particleSystem->setTemplateName( m_templateName.c_str() );
        }
    }

    //--------------------------------------------
    void PUParticleSystem::prepare()
    {
    }

    //--------------------------------------------
    void PUParticleSystem::start()
    {
        if( Thread::getCurrentTask() == Thread::Task::Render )
        {
            if( m_particleSystem )
                m_particleSystem->start();

            auto engine = core::ApplicationManager::instance();
            SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
            graphicsSystem->addFrameListener( m_frameListener );
        }
        else
        {
            StateMessageTypePtr message( new StateMessageType );
            FB_ASSERT( message );

            static const hash32 START_HASH = StringUtil::getHash( "start" );

            message->setType( START_HASH );
            message->setTypeValue( START_HASH );
            m_stateContext->addMessage( Thread::Task::Render, message );
        }
    }

    //--------------------------------------------
    void PUParticleSystem::stop()
    {
        if( Thread::getCurrentTask() == Thread::Task::Render )
        {
            if( m_particleSystem )
                m_particleSystem->stop();

            auto engine = core::ApplicationManager::instance();
            SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
            graphicsSystem->removeFrameListener( m_frameListener );
        }
        else
        {
            StateMessageTypePtr message( new StateMessageType );
            FB_ASSERT( message );

            static const hash32 STOP_HASH = StringUtil::getHash( "stop" );

            message->setType( STOP_HASH );
            message->setTypeValue( STOP_HASH );
            m_stateContext->addMessage( Thread::Task::Render, message );
        }
    }

    //--------------------------------------------
    void PUParticleSystem::stopFade()
    {
        if( m_particleSystem )
            m_particleSystem->stopFade();
    }

    //--------------------------------------------
    void PUParticleSystem::pause()
    {
    }

    //--------------------------------------------
    void PUParticleSystem::pause( f32 pauseTime )
    {
    }

    //--------------------------------------------
    void PUParticleSystem::resume()
    {
    }

    //--------------------------------------------
    void PUParticleSystem::setFastForward( f32 time, f32 interval )
    {
    }

    //--------------------------------------------
    f32 PUParticleSystem::getFastForwardTime() const
    {
        return 0.0f;
    }

    //--------------------------------------------
    f32 PUParticleSystem::getFastForwardInterval() const
    {
        return 0.0f;
    }

    //--------------------------------------------
    void PUParticleSystem::setTemplateName( const String &templateName )
    {
        // Ogre::String templateNameStr = templateName.c_str();
        // m_particleSystem->setTemplateName(templateNameStr);
    }

    //--------------------------------------------
    String PUParticleSystem::getTemplateName( void ) const
    {
        return StringUtil::EmptyString;
    }

    //--------------------------------------------
    void PUParticleSystem::setScale( const Vector3F &scale )
    {
        // if(m_particleSystem)
        //	m_particleSystem->setScale(Ogre::Vector3(scale));
    }

    //--------------------------------------------
    Vector3F PUParticleSystem::getScale() const
    {
        return Vector3F::zero();
    }

    //--------------------------------------------
    u32 PUParticleSystem::getState() const
    {
        if( m_particleSystem )
            return m_particleSystem->getState();

        return 0;
    }

    //--------------------------------------------
    SmartPtr<IParticleTechnique> PUParticleSystem::getTechnique( const String &name ) const
    {
        ParticleUniverse::ParticleTechnique *puTech = m_particleSystem->getTechnique( name.c_str() );
        return nullptr;  // SmartPtr<IParticleTechnique>(new PUParticleTechnique(puTech), true);
    }

    //--------------------------------------------
    s32 PUParticleSystem::getObject( u32 hash, SmartPtr<ISharedObject> &object ) const
    {
        Techniques::const_iterator it = m_techniques.find( hash );
        if( it != m_techniques.end() )
        {
            // object = it->second;
            return 0;
        }

        return 0;
    }

    //--------------------------------------------
    void PUParticleSystem::_updateRenderQueue( SmartPtr<IRenderQueue> queue )
    {
    }

    //--------------------------------------------
    ISceneNode *PUParticleSystem::getOwner() const
    {
        return nullptr;
    }

    //--------------------------------------------
    void PUParticleSystem::setOwner( ISceneNode *sceneNode )
    {
        // SmartPtr<CSceneManagerOgre> sceneManager = m_creator;
        // sceneManager->registerForUpdates(this);
    }

    void PUParticleSystem::load( void *data /*= nullptr*/ )
    {
        try
        {
            if( !m_particleSystem )
            {
                ParticleUniverse::ParticleSystemManager *particleSystemManager =
                    ParticleUniverse::ParticleSystemManager::getSingletonPtr();
                if( particleSystemManager )
                {
                    Ogre::SceneManager *sceneManager = nullptr;
                    m_creator->_getObject( (void **)&sceneManager );

                    m_particleSystem = particleSystemManager->createParticleSystem(
                        m_name.c_str(), m_templateName.c_str(), sceneManager );
                }
            }
        }
        catch( ... )
        {
            m_particleSystem = nullptr;
        }
    }

    void PUParticleSystem::unload()
    {
    }

    bool PUParticleSystem::isLoaded() const
    {
        return false;
    }

    //--------------------------------------------
    s32 PUParticleSystem::ScriptReceiver::setProperty( hash32 hash, const Parameter &param )
    {
        // if( hash == StringUtil::VISIBILITY_MASK_HASH )
        //{
        //	m_puSystem->setVisibilityFlags(param.data.iData);
        //	return 0;
        // }
        // else if( hash == StringUtil::VISIBLE_HASH )
        //{
        //	m_puSystem->setVisible(param.data.bData);
        //	return 0;
        // }

        return 0;
    }

    //--------------------------------------------
    s32 PUParticleSystem::ScriptReceiver::setProperty( hash32 id, const Parameters &params )
    {
        return 0;
    }

    //--------------------------------------------
    s32 PUParticleSystem::ScriptReceiver::getProperty( hash32 id, Parameter &param ) const
    {
        // if( id == StringUtil::VISIBILITY_MASK_HASH )
        //{
        //	param.data.iData = m_puSystem->getVisibilityFlags();
        //	return 0;
        // }

        return 0;
    }

    //--------------------------------------------
    s32 PUParticleSystem::ScriptReceiver::getProperty( hash32 id, Parameters &params ) const
    {
        return 0;
    }

    //--------------------------------------------
    s32 PUParticleSystem::ScriptReceiver::callFunction( u32 hashId, const Parameters &params,
                                                        Parameters &results )
    {
        // if( hashId == StringUtil::START_HASH )
        //{
        //	m_puSystem->getSystem()->start();
        //	return 0;
        // }
        // else if( hashId == StringUtil::STOP_HASH )
        //{
        //	m_puSystem->getSystem()->stop();
        //	return 0;
        // }
        // else if( hashId == StringUtil::UPDATE_HASH )
        //{
        //	SmartPtr<ITimer>& timer = IApplicationManager::instance()->getTimer();
        //	f64 dt = (f64)timer->getTimeIntervalMilliseconds() / 1000.0;
        //	m_puSystem->update(Thread::getCurrentTask(), 0.0, dt);
        //	return 0;
        // }

        return 0;
    }

    //--------------------------------------------
    PUParticleSystem::ParticleSystemFrameListener::ParticleSystemFrameListener(
        PUParticleSystem *puSystem ) :
        m_puSystem( puSystem )
    {
    }

    //--------------------------------------------
    PUParticleSystem::ParticleSystemFrameListener::~ParticleSystemFrameListener()
    {
    }

    //--------------------------------------------
    void PUParticleSystem::ParticleSystemFrameListener::frameEvent( const SmartPtr<IFrameEvent> &event )
    {
        hash32 eventType = event->getType();
        if( eventType == FRAME_EVENT_PRE_RENDER )
        {
            // m_puSystem->update(Thread::Task::TASK_ID_RENDER,
            // event->getTime(),event->getTimeInterval());
        }
    }

    //--------------------------------------------
    PUParticleSystem::ParticleSystemStateListener::ParticleSystemStateListener(
        PUParticleSystem *puSystem ) :
        m_puSystem( puSystem )
    {
    }

    //--------------------------------------------
    void PUParticleSystem::ParticleSystemStateListener::OnStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        // if(Thread::getCurrentTask() == Thread::Task::TASK_ID_RENDER)
        //{
        //	if(message->isExactly(StateMessageType::TYPE_INFO))
        //	{
        //		StateMessageTypePtr stateMessageType = message;
        //		if(stateMessageType->getType() == StringUtil::getHash("start"))
        //		{
        //			m_puSystem->start();
        //		}
        //		else if(stateMessageType->getType() == StringUtil::getHash("stop"))
        //		{
        //			m_puSystem->stop();
        //		}
        //		else if(stateMessageType->getType() == StringUtil::getHash("visibilityFlags"))
        //		{
        //			m_puSystem->setVisibilityFlags(stateMessageType->getTypeValue());
        //		}
        //	}
        //	else if(message->isExactly(StateMessageUIntValue::TYPE_INFO))
        //	{
        //		StateMessageUIntValuePtr stateMessageValue = message;
        //		if(stateMessageValue->getType() == STATE_MESSAGE_RENDER_QUEUE_GROUP)
        //		{
        //			m_puSystem->setRenderQueueGroup(stateMessageValue->getValue());
        //		}
        //	}
        //	else if(message->isExactly(StateMessageVisible::TYPE_INFO))
        //	{
        //		StateMessageVisiblePtr visibleMessage = message;
        //		m_puSystem->setVisible(visibleMessage->isVisible());
        //	}
        // }
    }

    //--------------------------------------------
    void PUParticleSystem::ParticleSystemStateListener::OnStateChanged( const SmartPtr<IState> &state )
    {
    }

    //--------------------------------------------
    void PUParticleSystem::ParticleSystemStateListener::OnQuery( SmartPtr<IStateQuery> &query )
    {
    }

    Vector3F PUParticleEmitter::getParticleSize() const
    {
        return Vector3F();
    }

    void PUParticleEmitter::setParticleSize( const Vector3F &val )
    {
    }

    Vector3F PUParticleEmitter::getParticleSizeMin() const
    {
        return Vector3F();
    }

    void PUParticleEmitter::setParticleSizeMin( const Vector3F &val )
    {
    }

    Vector3F PUParticleEmitter::getParticleSizeMax() const
    {
        return Vector3F();
    }

    void PUParticleEmitter::setParticleSizeMax( const Vector3F &val )
    {
    }

    Vector3F PUParticleEmitter::getDirection() const
    {
        return Vector3F();
    }

    void PUParticleEmitter::setDirection( const Vector3F &val )
    {
    }

    f32 PUParticleEmitter::getEmissionRate() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setEmissionRate( f32 val )
    {
    }

    f32 PUParticleEmitter::getEmissionRateMin() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setEmissionRateMin( f32 val )
    {
    }

    f32 PUParticleEmitter::getEmissionRateMax() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setEmissionRateMax( f32 val )
    {
    }

    f32 PUParticleEmitter::getTimeToLive() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setTimeToLive( f32 val )
    {
    }

    f32 PUParticleEmitter::getTimeToLiveMin() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setTimeToLiveMin( f32 val )
    {
    }

    f32 PUParticleEmitter::getTimeToLiveMax() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setTimeToLiveMax( f32 val )
    {
    }

    f32 PUParticleEmitter::getVelocity() const
    {
        return 0.f;
    }

    void PUParticleEmitter::setVelocity( f32 val )
    {
    }

    f32 PUParticleEmitter::getVelocityMin() const
    {
        return 0.0f;
    }

    void PUParticleEmitter::setVelocityMin( f32 val )
    {
    }

    f32 PUParticleEmitter::getVelocityMax() const
    {
        return 0.0f;
    }

    void PUParticleEmitter::setVelocityMax( f32 val )
    {
    }

    void PUParticleEmitter::calculateState( ParticlePtr particle, u32 stateIndex,
                                            void *data /*= nullptr*/ )
    {
    }

    SmartPtr<IParticleSystem> PUParticleEmitter::getParticleSystem() const
    {
        return nullptr;
    }

    void PUParticleEmitter::setParticleSystem( SmartPtr<IParticleSystem> val )
    {
    }

    void PUParticleAffector::calculateState( ParticlePtr particle, u32 stateIndex,
                                             void *data /*= nullptr*/ )
    {
    }

    SmartPtr<IParticleSystem> PUParticleAffector::getParticleSystem() const
    {
        return nullptr;
    }

    void PUParticleAffector::setParticleSystem( SmartPtr<IParticleSystem> val )
    {
    }

    void PUParticleTechnique::addEmitter( SmartPtr<IParticleEmitter> emitter )
    {
    }

    void PUParticleTechnique::removeEmitter( SmartPtr<IParticleEmitter> emitter )
    {
    }

    Array<SmartPtr<IParticleEmitter>> PUParticleTechnique::getParticleEmitters() const
    {
        return Array<SmartPtr<IParticleEmitter>>();
    }

    void PUParticleTechnique::addAffector( SmartPtr<IParticleAffector> affector )
    {
    }

    void PUParticleTechnique::removeAffector( SmartPtr<IParticleAffector> affector )
    {
    }

    Array<SmartPtr<IParticleAffector>> PUParticleTechnique::getParticleAffectors() const
    {
        return Array<SmartPtr<IParticleAffector>>();
    }

    void PUParticleTechnique::addRenderer( SmartPtr<IParticleRenderer> renderer )
    {
    }

    void PUParticleTechnique::removeRenderer( SmartPtr<IParticleRenderer> renderer )
    {
    }

    Array<SmartPtr<IParticleRenderer>> PUParticleTechnique::getParticleRenderers() const
    {
        return Array<SmartPtr<IParticleRenderer>>();
    }

    fb::SmartPtr<IParticleEmitter> PUParticleTechnique::getEmitter( hash32 hash ) const
    {
        return nullptr;
    }

    fb::SmartPtr<IParticleAffector> PUParticleTechnique::getAffector( hash32 hash ) const
    {
        return nullptr;
    }

    fb::SmartPtr<IParticleRenderer> PUParticleTechnique::getRenderer( hash32 hash ) const
    {
        return nullptr;
    }

    Array<ParticlePtr> PUParticleTechnique::getParticles() const
    {
        return Array<ParticlePtr>();
    }

    void PUParticleTechnique::setParticles( const Array<ParticlePtr> &val )
    {
    }

    void PUParticleTechnique::addParticle( ParticlePtr val )
    {
    }

    void PUParticleTechnique::removeParticle( ParticlePtr val )
    {
    }

    void PUParticleTechnique::clearParticles()
    {
    }

    void PUParticleTechnique::calculateState( ParticlePtr particle, u32 stateIndex,
                                              void *data /*= nullptr*/ )
    {
    }

    SmartPtr<IParticleSystem> PUParticleTechnique::getParticleSystem() const
    {
        return nullptr;
    }

    void PUParticleTechnique::setParticleSystem( SmartPtr<IParticleSystem> val )
    {
    }

#endif
}  // end namespace fb
