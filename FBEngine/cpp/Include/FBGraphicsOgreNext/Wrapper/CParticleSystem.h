#ifndef _CParticleSystem_H
#define _CParticleSystem_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Particle/IParticleSystem.h"
#include "FBCore/Interface/Graphics/IGraphicsScene.h"
#include "FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h"
#include "FBGraphicsOgreNext/Wrapper/CParticleNode.h"
#include <FBCore/Core/HashMap.h>

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include "ParticleUniverseSystemManager.h"
#endif

namespace fb
{

#if FB_OGRE_USE_PARTICLE_UNIVERSE
    class PUParticleEmitter : public CParticleNode<IParticleEmitter>
    {
    public:
        PUParticleEmitter()
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));
        }

        PUParticleEmitter( ParticleUniverse::ParticleEmitter *emitter ) : m_emitter( emitter )
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));
        }

        ~PUParticleEmitter()
        {
        }

        ParticleUniverse::ParticleEmitter *getEmitter() const
        {
            return m_emitter;
        }
        void setEmitter( ParticleUniverse::ParticleEmitter *val )
        {
            m_emitter = val;
        }

        //
        // IScriptObjects
        //

        /** Gets an object call script functions. */
        virtual SmartPtr<IScriptInvoker> &getInvoker()
        {
            return m_scriptInvoker;
        }

        /** Gets an object call script functions. */
        virtual const SmartPtr<IScriptInvoker> &getInvoker() const
        {
            return m_scriptInvoker;
        }

        /** Sets an object call script functions. */
        virtual void setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_scriptInvoker = invoker;
        }

        /** Gets an object to receive script calls. */
        virtual SmartPtr<IScriptReceiver> &getReceiver()
        {
            return m_scriptReceiver;
        }

        /** Gets an object to receive script calls. */
        virtual const SmartPtr<IScriptReceiver> &getReceiver() const
        {
            return m_scriptReceiver;
        }

        /** Sets an object to receive script calls. */
        virtual void setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_scriptReceiver = receiver;
        }

        virtual Vector3F getParticleSize() const;

        virtual void setParticleSize( const Vector3F &val );

        virtual Vector3F getParticleSizeMin() const;

        virtual void setParticleSizeMin( const Vector3F &val );

        virtual Vector3F getParticleSizeMax() const;

        virtual void setParticleSizeMax( const Vector3F &val );

        virtual Vector3F getDirection() const;

        virtual void setDirection( const Vector3F &val );

        virtual f32 getEmissionRate() const;

        virtual void setEmissionRate( f32 val );

        virtual f32 getEmissionRateMin() const;

        virtual void setEmissionRateMin( f32 val );

        virtual f32 getEmissionRateMax() const;

        virtual void setEmissionRateMax( f32 val );

        virtual f32 getTimeToLive() const;

        virtual void setTimeToLive( f32 val );

        virtual f32 getTimeToLiveMin() const;

        virtual void setTimeToLiveMin( f32 val );

        virtual f32 getTimeToLiveMax() const;

        virtual void setTimeToLiveMax( f32 val );

        virtual f32 getVelocity() const;

        virtual void setVelocity( f32 val );

        virtual f32 getVelocityMin() const;

        virtual void setVelocityMin( f32 val );

        virtual f32 getVelocityMax() const;

        virtual void setVelocityMax( f32 val );

        virtual void calculateState( ParticlePtr particle, u32 stateIndex, void *data = nullptr );

        virtual SmartPtr<IParticleSystem> getParticleSystem() const;
        virtual void setParticleSystem( SmartPtr<IParticleSystem> val );

    protected:
        class ScriptReceiver : public IScriptReceiver
        {
        public:
            ScriptReceiver( PUParticleEmitter *puEmitter ) : m_puEmitter( puEmitter )
            {
            }

            s32 setProperty( hash32 hash, const Parameters &values )
            {
                // if(hash == StringUtil::COLOUR_HASH || hash == StringUtil::COLOR_HASH)
                //{
                //	ColourF colour;
                //	colour.r = values[0].data.fData;
                //	colour.g = values[1].data.fData;
                //	colour.b = values[2].data.fData;
                //	colour.a = values[3].data.fData;
                //	m_puEmitter->getEmitter()->setParticleColour(Ogre::ColourValue(colour.r, colour.g,
                // colour.b, colour.a));

                //	return 0;
                //}

                return 0;
            }

            s32 setProperty( hash32 hash, void *value )
            {
                /*				if(hash == StringUtil::COLOUR_HASH || hash == StringUtil::COLOR_HASH)
                                {
                                    ColourF colour = *static_cast<ColourF*>(value);
                                    m_puEmitter->getEmitter()->setParticleColour(Ogre::ColourValue(colour.r,
                   colour.g, colour.b, colour.a));

                                    return 0;
                                }	*/

                return 0;
            }

        protected:
            PUParticleEmitter *m_puEmitter;
        };

        SmartPtr<IScriptInvoker> m_scriptInvoker;
        SmartPtr<IScriptReceiver> m_scriptReceiver;
        ParticleUniverse::ParticleEmitter *m_emitter;
    };

    class PUParticleAffector : public CParticleNode<IParticleAffector>
    {
    public:
        PUParticleAffector()
        {
        }
        PUParticleAffector( ParticleUniverse::ParticleAffector *affector ) : m_affector( affector )
        {
        }
        ~PUParticleAffector()
        {
        }

        virtual void calculateState( ParticlePtr particle, u32 stateIndex, void *data = nullptr );

        virtual SmartPtr<IParticleSystem> getParticleSystem() const;

        virtual void setParticleSystem( SmartPtr<IParticleSystem> val );

    protected:
        ParticleUniverse::ParticleAffector *m_affector;
    };

    class PUParticleRenderer : public CParticleNode<IParticleRenderer>
    {
    public:
        PUParticleRenderer()
        {
        }
        PUParticleRenderer( ParticleUniverse::ParticleRenderer *renderer ) : m_renderer( renderer )
        {
        }
        ~PUParticleRenderer()
        {
        }

    protected:
        ParticleUniverse::ParticleRenderer *m_renderer;
    };

    class PUParticleTechnique : public CParticleNode<IParticleTechnique>
    {
    public:
        PUParticleTechnique()
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));
        }

        PUParticleTechnique( ParticleUniverse::ParticleTechnique *puTech ) :
            m_particleTechnique( puTech )
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));

            // u32 numEmitters = m_particleTechnique->getNumEmitters();
            // for(u32 i=0; i<numEmitters; ++i)
            //{
            //	ParticleUniverse::ParticleEmitter* puEmitter = m_particleTechnique->getEmitter(i);

            //	SmartPtr<IParticleEmitter> emitterPtr(new PUParticleEmitter(puEmitter), true);
            //	m_particleEmitters[StringUtil::getHash(puEmitter->getName().c_str())] = emitterPtr;
            //}
        }

        ~PUParticleTechnique()
        {
        }

        ParticleUniverse::ParticleTechnique *getTechnique() const
        {
            return m_particleTechnique;
        }
        void setTechnique( ParticleUniverse::ParticleTechnique *val )
        {
            m_particleTechnique = val;
        }

        SmartPtr<IParticleEmitter> getEmitterByName( const String &name ) const
        {
            // ParticleUniverse::ParticleEmitter* puEmitter =
            // m_particleTechnique->getEmitter(name.c_str()); SmartPtr<IParticleEmitter> emitter(new
            // PUParticleEmitter(puEmitter), true); return emitter;

            return nullptr;
        }

        SmartPtr<IParticleAffector> getAffectorByName( const String &name ) const
        {
            // ParticleUniverse::ParticleAffector* puAffector =
            // m_particleTechnique->getAffector(name.c_str()); SmartPtr<IParticleAffector> affector(new
            // PUParticleAffector(puAffector), true); return affector;

            return nullptr;
        }

        SmartPtr<IParticleRenderer> getRendererByName( const String &name ) const
        {
            return SmartPtr<IParticleRenderer>();
        }

        s32 getObject( u32 hash, SmartPtr<ISharedObject> &object ) const
        {
            ParticleEmitters::const_iterator it = m_particleEmitters.find( hash );
            if( it != m_particleEmitters.end() )
            {
                // object = it->second;
                return 0;
            }

            return 0;
        }

        //
        // IScriptObjects
        //

        /** Gets an object call script functions. */
        virtual SmartPtr<IScriptInvoker> &getInvoker()
        {
            return m_scriptInvoker;
        }

        /** Gets an object call script functions. */
        virtual const SmartPtr<IScriptInvoker> &getInvoker() const
        {
            return m_scriptInvoker;
        }

        /** Sets an object call script functions. */
        virtual void setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_scriptInvoker = invoker;
        }

        /** Gets an object to receive script calls. */
        virtual SmartPtr<IScriptReceiver> &getReceiver()
        {
            return m_scriptReceiver;
        }

        /** Gets an object to receive script calls. */
        virtual const SmartPtr<IScriptReceiver> &getReceiver() const
        {
            return m_scriptReceiver;
        }

        /** Sets an object to receive script calls. */
        virtual void setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_scriptReceiver = receiver;
        }

        virtual void addEmitter( SmartPtr<IParticleEmitter> emitter );

        virtual void removeEmitter( SmartPtr<IParticleEmitter> emitter );

        virtual Array<SmartPtr<IParticleEmitter>> getParticleEmitters() const;

        virtual void addAffector( SmartPtr<IParticleAffector> affector );

        virtual void removeAffector( SmartPtr<IParticleAffector> affector );

        virtual Array<SmartPtr<IParticleAffector>> getParticleAffectors() const;

        virtual void addRenderer( SmartPtr<IParticleRenderer> renderer );

        virtual void removeRenderer( SmartPtr<IParticleRenderer> renderer );

        virtual Array<SmartPtr<IParticleRenderer>> getParticleRenderers() const;

        virtual SmartPtr<IParticleEmitter> getEmitter( hash32 hash ) const;

        virtual SmartPtr<IParticleAffector> getAffector( hash32 hash ) const;

        virtual SmartPtr<IParticleRenderer> getRenderer( hash32 hash ) const;

        virtual Array<ParticlePtr> getParticles() const;

        virtual void setParticles( const Array<ParticlePtr> &val );

        virtual void addParticle( ParticlePtr val );

        virtual void removeParticle( ParticlePtr val );

        virtual void clearParticles();

        virtual void calculateState( ParticlePtr particle, u32 stateIndex, void *data = nullptr );

        virtual SmartPtr<IParticleSystem> getParticleSystem() const;

        virtual void setParticleSystem( SmartPtr<IParticleSystem> val );

    protected:
        class ScriptReceiver : public IScriptReceiver
        {
        public:
            ScriptReceiver( PUParticleTechnique *puTechnique ) : m_puTechnique( puTechnique )
            {
            }

            s32 setProperty( hash32 hashId, const String &value )
            {
                // if(hashId == StringUtil::MATERIAL_HASH)
                //{
                //	m_puTechnique->getTechnique()->setMaterialName(value.c_str());
                //	return 0;
                // }

                return 0;
            }

            s32 getProperty( const String &name, const String &value )
            {
                return 0;
            }

        protected:
            PUParticleTechnique *m_puTechnique;
        };

        SmartPtr<IScriptInvoker> m_scriptInvoker;
        SmartPtr<IScriptReceiver> m_scriptReceiver;

        ParticleUniverse::ParticleTechnique *m_particleTechnique;

        typedef HashMap<u32, SmartPtr<IParticleEmitter>> ParticleEmitters;
        ParticleEmitters m_particleEmitters;
    };

    //--------------------------------------------
    class PUParticleSystem : public CGraphicsObjectOgreNext<IParticleSystem>
    {
    public:
        static const hash32 STATE_MESSAGE_RENDER_QUEUE_GROUP;

        PUParticleSystem( SmartPtr<render::IGraphicsScene> creator );
        ~PUParticleSystem();

        void handleMessage( const SmartPtr<IMessage> &message );

        void initialise( ParticleUniverse::ParticleSystem *particleSystem );

        void update( const s32 &task, const time_interval &t, const time_interval &dt );

        // IGraphicsObject functions
        void setName( const String &name );
        String getName() const;
        u32 getId() const;

        void detachFromParent()
        {
        }
        void _attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        void setMaterialName( const String &materialName, s32 index = -1 );
        String getMaterialName( s32 index = -1 ) const;

        void setCastShadows( bool castShadows );
        bool getCastShadows() const;

        void setRecieveShadows( bool recieveShadows );
        bool getRecieveShadows() const;

        void setVisible( bool isVisible );
        bool isVisible() const;

        void setRenderQueueGroup( u8 renderQueue );

        void setVisibilityFlags( u32 flags );
        u32 getVisibilityFlags() const;

        SmartPtr<IGraphicsObject> clone( const String &name = StringUtil::EmptyString ) const;

        void _getObject( void **ppObject ) const;

        const String &getComponentType() const;

        void setPropertyValue( const String &name, const String &value );
        bool getPropertyValue( const String &name, String &value );

        void setProperties( const Properties &propertyGroup );
        bool getProperties( Properties &propertyGroup, u32 flags = AllProperties ) const;

        ISceneNode *getOwner() const;
        void setOwner( ISceneNode *sceneNode );

        // IParticleSystem functions
        void reload();
        void prepare();
        void start();
        void stop();
        void stopFade();
        void pause();
        void pause( f32 pauseTime );
        void resume();

        void setFastForward( f32 time, f32 interval );
        f32 getFastForwardTime() const;
        f32 getFastForwardInterval() const;

        void setTemplateName( const String &templateName );
        String getTemplateName( void ) const;

        void setScale( const Vector3F &scale );
        Vector3F getScale() const;

        u32 getState() const;

        SmartPtr<IParticleTechnique> getTechnique( const String &name ) const;

        ParticleUniverse::ParticleSystem *getSystem() const
        {
            return m_particleSystem;
        }
        void setParticleSystem( ParticleUniverse::ParticleSystem *val )
        {
            m_particleSystem = val;
        }

        //
        // IScriptObjects
        //

        /** Gets an object call script functions. */
        virtual SmartPtr<IScriptInvoker> &getInvoker()
        {
            return m_scriptInvoker;
        }

        /** Gets an object call script functions. */
        virtual const SmartPtr<IScriptInvoker> &getInvoker() const
        {
            return m_scriptInvoker;
        }

        /** Sets an object call script functions. */
        virtual void setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_scriptInvoker = invoker;
        }

        /** Gets an object to receive script calls. */
        virtual SmartPtr<IScriptReceiver> &getReceiver()
        {
            return m_scriptReceiver;
        }

        /** Gets an object to receive script calls. */
        virtual const SmartPtr<IScriptReceiver> &getReceiver() const
        {
            return m_scriptReceiver;
        }

        /** Sets an object to receive script calls. */
        virtual void setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_scriptReceiver = receiver;
        }

        s32 getObject( u32 hash, SmartPtr<ISharedObject> &object ) const;

        void _updateRenderQueue( SmartPtr<IRenderQueue> queue );

        SmartPtr<IStateContext> &getStateObject()
        {
            return m_stateObject;
        }
        const SmartPtr<IStateContext> &getStateObject() const
        {
            return m_stateObject;
        }
        void setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        virtual SmartPtr<render::IGraphicsScene> getSceneManager() const
        {
            return 0;
        }
        virtual void setSceneManager( SmartPtr<render::IGraphicsScene> val )
        {
        }

        virtual void load( void *data = nullptr );

        virtual void unload();

        virtual bool isLoaded() const;

        void addListener( SmartPtr<IParticleSystemListener> listener )
        {
        }
        void removeListener( SmartPtr<IParticleSystemListener> listener )
        {
        }

    protected:
        class ScriptReceiver : public IScriptReceiver
        {
        public:
            ScriptReceiver( PUParticleSystem *puSystem ) : m_puSystem( puSystem )
            {
            }

            s32 setProperty( hash32 id, const Parameter &param );
            s32 setProperty( hash32 id, const Parameters &params );
            s32 getProperty( hash32 id, Parameter &param ) const;
            s32 getProperty( hash32 id, Parameters &params ) const;
            s32 callFunction( u32 hashId, const Parameters &params, Parameters &results );

        private:
            PUParticleSystem *m_puSystem;
        };

        class ParticleSystemFrameListener : public IFrameListener
        {
        public:
            ParticleSystemFrameListener( PUParticleSystem *puSystem );
            ~ParticleSystemFrameListener();

            virtual void frameEvent( const SmartPtr<IFrameEvent> &event );

            PUParticleSystem *m_puSystem;
        };

        class ParticleSystemStateListener : public IStateListener
        {
        public:
            ParticleSystemStateListener( PUParticleSystem *puSystem );

            void OnStateChanged( const SmartPtr<IStateMessage> &message );
            void OnStateChanged( const SmartPtr<IState> &state );
            void OnQuery( SmartPtr<IStateQuery> &query );

            PUParticleSystem *m_puSystem;
        };

        SmartPtr<IStateContext> m_stateObject;

        SmartPtr<IScriptInvoker> m_scriptInvoker;
        SmartPtr<IScriptReceiver> m_scriptReceiver;

        ParticleUniverse::ParticleSystem *m_particleSystem;

        SmartPtr<IFrameListener> m_frameListener;

        typedef HashMap<u32, SmartPtr<IParticleTechnique>> Techniques;
        Techniques m_techniques;

        SmartPtr<render::IGraphicsScene> m_creator;
        String m_name;
        String m_templateName;

        atomic_bool m_visible;
        atomic_u32 m_renderQueueGroup;
    };

    typedef SmartPtr<PUParticleSystem> PUParticleSystemPtr;

#endif
}  // end namespace fb

#endif
