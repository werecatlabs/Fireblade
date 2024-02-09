#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/GraphicsScene.h>

#include "FBCore/State/States/AmbientLightState.h"

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED( fb, GraphicsScene, SharedGraphicsObject<IGraphicsScene> );

    GraphicsScene::GraphicsScene()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "GraphicsScene" );
        }
    }

    GraphicsScene::~GraphicsScene() = default;

    void GraphicsScene::load( SmartPtr<ISharedObject> data )
    {
    }

    void GraphicsScene::unload( SmartPtr<ISharedObject> data )
    {
        SharedGraphicsObject<IGraphicsScene>::unload( data );
    }

    void GraphicsScene::setName( const String &name )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto handle = getHandle() )
        {
            handle->setName( name );
        }
    }

    auto GraphicsScene::getName() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto handle = getHandle() )
        {
            return handle->getName();
        }

        return {};
    }

    ColourF GraphicsScene::getUpperHemisphere() const
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                return state->getUpperHemisphere();
            }
        }

        return ColourF::White;
    }

    void GraphicsScene::setUpperHemisphere( const ColourF &upperHemisphere )
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                state->setUpperHemisphere( upperHemisphere );
            }
        }
    }

    ColourF GraphicsScene::getLowerHemisphere() const
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                return state->getLowerHemisphere();
            }
        }

        return ColourF::White;
    }

    void GraphicsScene::setLowerHemisphere( const ColourF &lowerHemisphere )
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                state->setLowerHemisphere( lowerHemisphere );
            }
        }
    }

    Vector3<real_Num> GraphicsScene::getHemisphereDir() const
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                return state->getHemisphereDir();
            }
        }

        return Vector3<real_Num>::unit();
    }

    void GraphicsScene::setHemisphereDir( Vector3<real_Num> hemisphereDir )
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                state->setHemisphereDir( hemisphereDir );
            }
        }
    }

    f32 GraphicsScene::getEnvmapScale() const
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                return state->getEnvmapScale();
            }
        }

        return 1.0f;
    }

    void GraphicsScene::setEnvmapScale( f32 envmapScale )
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<AmbientLightState>() )
            {
                state->setEnvmapScale( envmapScale );
            }
        }
    }

    void GraphicsScene::unlock()
    {
        m_mutex.unlock();
    }

    void GraphicsScene::lock()
    {
        m_mutex.lock();
    }

    void GraphicsScene::setType( const String &type )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_type = type;
    }

    auto GraphicsScene::getType() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_type;
    }

}  // namespace fb::render
