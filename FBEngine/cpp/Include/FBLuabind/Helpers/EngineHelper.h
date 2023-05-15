#ifndef EngineHelper_h__
#define EngineHelper_h__

#include <FBCore/Interface/IApplicationManager.h>

namespace fb
{

    class EngineHelper
    {
    public:
        static core::IApplicationManager* getSingleton();

        static SmartPtr<ITimer> getTimer( core::IApplicationManager *engine );

        static ITimer* getTimerSingleton();

        //static SmartPtr<IStateManager> getStateManager( core::IApplicationManager *engine );

        ////static FactoryPtr getFactory(Engine* engine);
        //static boost::shared_ptr<IFileSystem> getFileSystem( core::IApplicationManager *engine );

        //static GraphicsSystemPtr getGraphicsSystem( core::IApplicationManager *engine );
        //static InputManagerPtr getInputManager( core::IApplicationManager *engine );
        //static MapManagerPtr getMapManager( core::IApplicationManager *engine );

        //static EntityManagerPtr getEntityManager( core::IApplicationManager *engine );

        //static PhysicsManager2Ptr getPhysicsManager2( core::IApplicationManager *engine );

        //static PhysicsManager3Ptr getPhysicsManager3( core::IApplicationManager *engine );

        //static PoolPtr getPool( core::IApplicationManager *engine );

        //static SmartPtr<ITimer> getMainTimer( core::IApplicationManager *engine );

        //static MessageManagerPtr getMessageManager( core::IApplicationManager *engine );

        //static boost::shared_ptr<IComponent> getComponentHash( core::IApplicationManager *system, lua_Integer hash );

        //static boost::shared_ptr<IComponentContainer> _getComponents( core::IApplicationManager *system );

        //static boost::shared_ptr<IComponent> getComponent( core::IApplicationManager *system, const char *type );

        //static ICameraManager *_getCameraControllerMgr( core::IApplicationManager *engine );

        //static void printLuaMessage( const char *msg );

        //static IStateManager *_getStateManager( core::IApplicationManager *system );

        //static IGraphicsSystem *_getGraphicsSystem( core::IApplicationManager *system );

        //static IInputManager *_getInputManager( core::IApplicationManager *system );

        //static IEntityManager *_getEntityManager( core::IApplicationManager *system );

        //static IPhysicsManager2 *_getPhysicsManager2( core::IApplicationManager *system );

        //static IPhysicsManager3 *_getPhysicsManager3( core::IApplicationManager *system );

        //static ISoundManager *_getSoundManager( core::IApplicationManager *system );

        //static IFlashControlManager *_getFlashControlManager( core::IApplicationManager *system );

        //static IVideoManager *_getVideoManager( core::IApplicationManager *system );

        //static ICombatManager2 *_getCombatManager2( core::IApplicationManager *system );

        //static IFileSystem *_getFileSystem( core::IApplicationManager *system );

        //static IProjectileManager2 *_getProjectileManager2( core::IApplicationManager *system );

        //static IGameManager *_getGameSceneManager( core::IApplicationManager *system );

        //static IGUIManager *_getGUIManager( core::IApplicationManager *system );

        //static IAiManager *_getAiManager( core::IApplicationManager *system );

        //static ISpecialFxManager *_getSpecialFxManager( core::IApplicationManager *system );

        //static IFactory* _getFactory(Engine* engine);
    };

}  // end namespace fb

#endif  // EngineHelper_h__
