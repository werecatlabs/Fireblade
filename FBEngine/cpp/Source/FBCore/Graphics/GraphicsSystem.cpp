#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/GraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IWindow.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, GraphicsSystem, SharedGraphicsObject<IGraphicsSystem> );

    GraphicsSystem::GraphicsSystem()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "GraphicsSystem" );
        }

        auto scenes = fb::make_shared<ConcurrentArray<SmartPtr<IGraphicsScene>>>();
        setScenesPtr( scenes );
    }

    GraphicsSystem::~GraphicsSystem() = default;

    void GraphicsSystem::load( SmartPtr<ISharedObject> data )
    {
    }

    void GraphicsSystem::unload( SmartPtr<ISharedObject> data )
    {
    }

    void GraphicsSystem::lock()
    {
        m_mutex.lock();
    }

    void GraphicsSystem::unlock()
    {
        m_mutex.unlock();
    }

    auto GraphicsSystem::getWindows() const -> Array<SmartPtr<IWindow>>
    {
        return m_windows;
    }

    auto GraphicsSystem::getSceneManagers() const -> Array<SmartPtr<IGraphicsScene>>
    {
        if( auto p = getScenesPtr() )
        {
            auto &scenes = *p;
            return Array<SmartPtr<IGraphicsScene>>( scenes.begin(), scenes.end() );
        }

        return {};
    }

    auto GraphicsSystem::getScenesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>>
    {
        return m_scenes;
    }

    void GraphicsSystem::setScenesPtr( SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> scenes )
    {
        m_scenes = scenes;
    }

    void GraphicsSystem::addScenePtr( SmartPtr<IGraphicsScene> sceneManager )
    {
        auto p = getScenesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IGraphicsScene>>>();
            setScenesPtr( p );
        }

        if( p )
        {
            auto &scenes = *p;
            scenes.push_back( sceneManager );
        }
    }

    void GraphicsSystem::removeScenePtr( SmartPtr<IGraphicsScene> sceneManager )
    {
        if( auto p = getScenesPtr() )
        {
            auto scenes = Array<SmartPtr<IGraphicsScene>>( p->begin(), p->end() );
            scenes.erase( std::remove( scenes.begin(), scenes.end(), sceneManager ), scenes.end() );

            auto newScenes = fb::make_shared<ConcurrentArray<SmartPtr<IGraphicsScene>>>( scenes.begin(),
                                                                                         scenes.end() );
            setScenesPtr( newScenes );
        }
    }

    auto GraphicsSystem::getGraphicsScene( const String &name ) const -> SmartPtr<IGraphicsScene>
    {
        if( auto p = getScenesPtr() )
        {
            auto &scenes = *p;
            for( auto sceneManager : scenes )
            {
                if( sceneManager->getName() == name )
                {
                    return sceneManager;
                }
            }
        }

        return nullptr;
    }

    auto GraphicsSystem::getGraphicsScene() const -> SmartPtr<IGraphicsScene>
    {
        return m_defaultSceneManager;
    }

    void GraphicsSystem::setGraphicsScene( SmartPtr<IGraphicsScene> smgr )
    {
        m_defaultSceneManager = smgr;
    }

    auto GraphicsSystem::getGraphicsSceneById( hash32 id ) const -> SmartPtr<IGraphicsScene>
    {
        if( auto p = getScenesPtr() )
        {
            auto &scenes = *p;
            for( auto sceneManager : scenes )
            {
                auto name = sceneManager->getName();
                auto hash = StringUtil::getHash( name );
                if( hash == id )
                {
                    return sceneManager;
                }
            }
        }

        return nullptr;
    }
}  // namespace fb::render
