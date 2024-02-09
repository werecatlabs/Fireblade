#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/GraphicsScene.h>

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
        return m_upperHemisphere;
    }

    void GraphicsScene::setUpperHemisphere( const ColourF &upperHemisphere )
    {
        m_upperHemisphere = upperHemisphere;
    }

    ColourF GraphicsScene::getLowerHemisphere() const
    {
        return m_lowerHemisphere;
    }

    void GraphicsScene::setLowerHemisphere( const ColourF &lowerHemisphere )
    {
        m_lowerHemisphere = lowerHemisphere;
    }

    Vector3<real_Num> GraphicsScene::getHemisphereDir() const
    {
        return m_hemisphereDir;
    }

    void GraphicsScene::setHemisphereDir( Vector3<real_Num> hemisphereDir )
    {
        m_hemisphereDir = hemisphereDir;
    }

    f32 GraphicsScene::getEnvmapScale() const
    {
        return m_envmapScale;
    }

    void GraphicsScene::setEnvmapScale( f32 envmapScale )
    {
        m_envmapScale = envmapScale;
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
