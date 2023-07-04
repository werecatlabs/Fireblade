#include <FBGraphics/Wrapper/CGraphicsSystem.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CGraphicsSystem::CGraphicsSystem()
        {
            auto scenes = fb::make_shared<ConcurrentArray<SmartPtr<IGraphicsScene>>>();
            setScenesPtr( scenes );
        }

        CGraphicsSystem::~CGraphicsSystem()
        {
        }

        void CGraphicsSystem::load( SmartPtr<ISharedObject> data )
        {
        }

        void CGraphicsSystem::unload( SmartPtr<ISharedObject> data )
        {
        }

        void CGraphicsSystem::lock()
        {
            m_mutex.lock();
        }

        void CGraphicsSystem::unlock()
        {
            m_mutex.unlock();
        }

        Array<SmartPtr<IWindow>> CGraphicsSystem::getWindows() const
        {
            return m_windows;
        }

        Array<SmartPtr<IGraphicsScene>> CGraphicsSystem::getSceneManagers() const
        {
            if( auto p = getScenesPtr() )
            {
                auto &scenes = *p;
                return Array<SmartPtr<IGraphicsScene>>( scenes.begin(), scenes.end() );
            }

            return Array<SmartPtr<IGraphicsScene>>();
        }

        SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> CGraphicsSystem::getScenesPtr() const
        {
            return m_scenes;
        }

        void CGraphicsSystem::setScenesPtr( SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> scenes )
        {
            m_scenes = scenes;
        }

        void CGraphicsSystem::addScenePtr( SmartPtr<IGraphicsScene> sceneManager )
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

        void CGraphicsSystem::removeScenePtr( SmartPtr<IGraphicsScene> sceneManager )
        {
            if( auto p = getScenesPtr() )
            {
                auto scenes = Array<SmartPtr<IGraphicsScene>>( p->begin(), p->end() );
                scenes.erase( std::remove( scenes.begin(), scenes.end(), sceneManager ), scenes.end() );

                auto newScenes = fb::make_shared<ConcurrentArray<SmartPtr<IGraphicsScene>>>(
                    scenes.begin(), scenes.end() );
                setScenesPtr( newScenes );
            }
        }

        SmartPtr<IGraphicsScene> CGraphicsSystem::getGraphicsScene( const String &name ) const
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

        SmartPtr<IGraphicsScene> CGraphicsSystem::getGraphicsScene() const
        {
            return m_defaultSceneManager;
        }

        void CGraphicsSystem::setGraphicsScene( SmartPtr<IGraphicsScene> smgr )
        {
            m_defaultSceneManager = smgr;
        }

        SmartPtr<IGraphicsScene> CGraphicsSystem::getGraphicsSceneById( hash32 id ) const
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
    }  // namespace render
}  // namespace fb
