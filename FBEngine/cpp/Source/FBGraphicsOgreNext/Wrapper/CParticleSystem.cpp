#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CParticleSystem.h"
#include "FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h"

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include "ParticleAffectors/ParticleUniverseColourAffector.h"
#endif

#include "FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h"
#include <FBCore/Interface/Graphics/IParticleNode.h>
#include <Ogre.h>

namespace fb::render
{

    CParticleSystem::CParticleSystem() = default;

    CParticleSystem::~CParticleSystem() = default;

    void CParticleSystem::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            Ogre::SceneManager *smgr = nullptr;

            if( auto creator = getCreator() )
            {
                creator->_getObject( reinterpret_cast<void **>( &smgr ) );
            }

            Ogre::ParticleSystem *particleSystem =
                smgr->createParticleSystem( 100, "MyParticleTemplate" );
            m_particleSystem = particleSystem;

            //auto *emitter = particleSystem->getEmitter( 0 );

            //// Set emitter properties
            //emitter->setEmissionRate( 100 );
            //emitter->setParticleVelocity( 10 );
            //// ... other properties

            //// Set material for the particles
            //particleSystem->setMaterialName( "Examples/Smoke" );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CParticleSystem::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CParticleSystem::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
    {
        return nullptr;
    }

    void CParticleSystem::_getObject( void **ppObject ) const
    {
        *ppObject = m_particleSystem;
    }

}  // namespace fb::render
