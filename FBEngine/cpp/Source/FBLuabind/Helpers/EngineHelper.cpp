#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Helpers/EngineHelper.h"
#include <FBCore/FBCore.h>

namespace fb
{

    core::IApplicationManager* EngineHelper::getSingleton()
    {
        auto applicationManager = core::IApplicationManager::instance();
        return applicationManager.get();
    }

    SmartPtr<ITimer> EngineHelper::getTimer( core::IApplicationManager *engine )
    {
        return engine->getTimer();
    }

    ITimer* EngineHelper::getTimerSingleton()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto timer = applicationManager->getTimer();
        return timer.get();
    }

    //SmartPtr<IStateManager> EngineHelper::getStateManager( core::IApplicationManager *engine )
    //{
    //    return engine->getStateManager();
    //}

    //fb::FactoryPtr EngineHelper::getFactory( Engine* engine )
    //{
    //	return engine->getFactory();
    //}

    //GraphicsSystemPtr EngineHelper::getGraphicsSystem( core::IApplicationManager *engine )
    //{
    //    return engine->getGraphicsSystem();
    //}

    //MapManagerPtr EngineHelper::getMapManager( core::IApplicationManager *engine )
    //{
    //    return engine->getMapManager();
    //}

    //EntityManagerPtr EngineHelper::getEntityManager( core::IApplicationManager *engine )
    //{
    //    return engine->getEntityManager();
    //}

    ////boost::shared_ptr<IFileSystem> EngineHelper::getFileSystem( Engine* engine )
    ////{
    ////	return engine->getFileSystem();
    ////}

    //fb::InputManagerPtr EngineHelper::getInputManager( core::IApplicationManager *engine )
    //{
    //    return engine->getInputManager();
    //}

    //fb::PhysicsManager2Ptr EngineHelper::getPhysicsManager2( core::IApplicationManager *engine )
    //{
    //    return engine->getPhysicsManager2();
    //}

    //fb::PhysicsManager3Ptr EngineHelper::getPhysicsManager3( core::IApplicationManager *engine )
    //{
    //    return engine->getPhysicsManager3();
    //}

    //fb::PoolPtr EngineHelper::getPool( core::IApplicationManager *engine )
    //{
    //    return engine->getPool();
    //}

    //fb::SmartPtr<ITimer> EngineHelper::getMainTimer( core::IApplicationManager *engine )
    //{
    //    return engine->getMainTimer();
    //}

    //fb::MessageManagerPtr EngineHelper::getMessageManager( core::IApplicationManager *engine )
    //{
    //    return engine->getMessageManager();
    //}

    //boost::shared_ptr<IComponent> EngineHelper::getComponentHash( core::IApplicationManager *system, lua_Integer hash )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    boost::shared_ptr<IComponentContainer> &components = system->getComponents();
    //    if( components )
    //    {
    //        boost::shared_ptr<IComponent> component = components->getComponentById( id );
    //        if( component )
    //        {
    //            return component;
    //        }
    //    }

    //    String msg = String( "Could find component: " ) + StringUtil::toString( id ) + String( " " );
    //    msg += Engine::getSingletonPtr()->getScriptManager()->getDebugInfo();
    //    FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

    //    return nullptr;
    //}

    //boost::shared_ptr<IComponentContainer> EngineHelper::_getComponents( core::IApplicationManager *system )
    //{
    //    boost::shared_ptr<IComponentContainer> &components = system->getComponents();
    //    return components;
    //}

    //boost::shared_ptr<IComponent> EngineHelper::getComponent( core::IApplicationManager *system, const char *type )
    //{
    //    auto hash = StringUtil::getHash( type );
    //    lua_Integer id = *reinterpret_cast<lua_Integer *>( &hash );
    //    return getComponentHash( system, id );
    //}

    //ICameraManager *EngineHelper::_getCameraControllerMgr( core::IApplicationManager *engine )
    //{
    //    return engine->getCameraManager().get();
    //}

    //void EngineHelper::printLuaMessage( const char *msg )
    //{
    //    printf( msg );
    //}

    //IStateManager *EngineHelper::_getStateManager( core::IApplicationManager *system )
    //{
    //    return system->getStateManager().get();
    //}

    //IGraphicsSystem *EngineHelper::_getGraphicsSystem( core::IApplicationManager *system )
    //{
    //    return system->getGraphicsSystem().get();
    //}

    //IInputManager *EngineHelper::_getInputManager( core::IApplicationManager *system )
    //{
    //    return system->getInputManager().get();
    //}

    //IEntityManager *EngineHelper::_getEntityManager( core::IApplicationManager *system )
    //{
    //    return system->getEntityManager().get();
    //}

    //IPhysicsManager2 *EngineHelper::_getPhysicsManager2( core::IApplicationManager *system )
    //{
    //    return system->getPhysicsManager2().get();
    //}

    //IPhysicsManager3 *EngineHelper::_getPhysicsManager3( core::IApplicationManager *system )
    //{
    //    return system->getPhysicsManager3().get();
    //}

    //ISoundManager *EngineHelper::_getSoundManager( core::IApplicationManager *system )
    //{
    //    return system->getSoundManager().get();
    //}

    //IFlashControlManager *EngineHelper::_getFlashControlManager( core::IApplicationManager *system )
    //{
    //    return system->getFlashControlManager().get();
    //}

    //IVideoManager *EngineHelper::_getVideoManager( core::IApplicationManager *system )
    //{
    //    return system->getVideoManager().get();
    //}

    //ICombatManager2 *EngineHelper::_getCombatManager2( core::IApplicationManager *system )
    //{
    //    return system->getCombatManager2().get();
    //}

    //IFileSystem *EngineHelper::_getFileSystem( core::IApplicationManager *system )
    //{
    //    return 0;  // system->getFileSystem().get();
    //}

    //IProjectileManager2 *EngineHelper::_getProjectileManager2( core::IApplicationManager *system )
    //{
    //    return system->getProjectileManager2().get();
    //}

    //IGameManager *EngineHelper::_getGameSceneManager( core::IApplicationManager *system )
    //{
    //    return system->getGameManager().get();
    //}

    //IGUIManager *EngineHelper::_getGUIManager( core::IApplicationManager *system )
    //{
    //    return system->getGUIManager().get();
    //}

    //IAiManager *EngineHelper::_getAiManager( core::IApplicationManager *system )
    //{
    //    return system->getAiManager().get();
    //}

    //ISpecialFxManager *EngineHelper::_getSpecialFxManager( core::IApplicationManager *system )
    //{
    //    return system->getSpecialFxManager().get();
    //}

    //IFactory* EngineHelper::_getFactory( Engine* engine )
    //{
    //	return engine->getFactory().get();
    //}

}  // end namespace fb
