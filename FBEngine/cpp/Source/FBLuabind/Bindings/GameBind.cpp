#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/GameBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"
#include <FBApplication/FBApplication.h>

namespace fb
{

    //boost::shared_ptr<IComponentContainer> _getComponents( IWeapon *weapon )
    //{
    //    return weapon->getComponents();
    //}

    //IScriptObject* _getUserData(IWeapon* weapon)
    //{
    //	return static_cast<IScriptObject*>(weapon->getUserData());
    //}

    //void _setUserData(IWeapon* weapon, IScriptObject* object)
    //{
    //	weapon->setUserData(object);
    //}

    //WeaponPtr _createWeapon(IFactory* factory, const char* weaponType)
    //{
    //	return factory->create(weaponType);
    //}

    //WeaponFireArm2Ptr _createFireArm(IFactory* factory, const char* weaponType)
    //{
    //	return factory->create(weaponType);
    //}

    //WeaponFireArm2Ptr _createFireArmByTypeId(IFactory* factory, lua_Integer weaponType)
    //{
    //	return factory->createById(weaponType);
    //}

    //GameWorldPtr _getGameWorld( IGameManager *gameSceneMgr, lua_Integer id )
    //{
    //    return gameSceneMgr->getGameWorld( id );
    //}

    //FSMContainerPtr _getFSMContainer( IGameManager *mgr )
    //{
    //    return mgr->getFSMContainer();
    //}

    //FSMContainerPtr _sceneGetFSMContainer( IScene *scene )
    //{
    //    return scene->getFSMs();
    //}

    void bindGame( lua_State *L )
    {
        using namespace luabind;

        //module(L)
        //	[
        //		class_<IApplication, IScriptObject, ApplicationPtr>( "IApplication" )
        //		.def("shutdown", &IApplication::shutdown)
        //		.def("run", &IApplication::run)
        //		.def("getEngine", &IApplication::getEngine)
        //		.def("setEngine", &IApplication::setEngine)
        //		.def("getFSM", &IApplication::getFSM)
        //		.def("setFSM", &IApplication::setFSM)
        //	];

        //module( L )[class_<IGameWorld, IScriptObject, GameWorldPtr>( "GameWorld" )
        //                .def( "query", &IGameWorld::query )];

        //module( L )[class_<IGameManager, IScriptObject, GameManagerPtr>( "GameManager" )
        //                .def( "handleMessage", &IGameManager::handleMessage )
        //                .def( "getGameWorld", _getGameWorld )

        //                .def( "setFSMs", &IGameManager::setFSMContainer )
        //                .def( "getFSMs", _getFSMContainer )];

        //module( L )[class_<IProjectileManager2, IScriptObject, ProjectileManager2Ptr>(
        //    "ProjectileManager2" )];

        //module( L )[class_<ICameraManager, IScriptObject, CameraManagerPtr>( "CameraControllerMgr" )];

        //module( L )[class_<IMap, IScriptObject, MapPtr>( "IMap" ).def( "load", &IMap::load )];

        //module( L )[class_<IMapManager, IScriptObject, MapManagerPtr>( "IMapManager" )
        //                .def( "addMap", &IMapManager::addMap )];

        //module( L )[class_<IAction, IScriptObject, ActionPtr>( "IAction" )
        //                .def( "execute", &IAction::execute )
        //                .def( "getOwner", &IAction::getOwner )
        //                .def( "setOwner", &IAction::setOwner )];

        //module(
        //    L )[class_<IActionController, IScriptObject, ActionControllerPtr>( "IActionController" )
        //            .def( "addAction", &IActionController::addAction )
        //            .def( "removeAction", &IActionController::removeAction )
        //            .def( "getActions", &IActionController::getActions )
        //            .def( "executeById", &IActionController::executeById )
        //            .def( "executeByName", &IActionController::executeByName )

        //            .def( "stopById", &IActionController::stopById )
        //            .def( "stopByName", &IActionController::stopByName )
        //            .def( "stopAll", &IActionController::stopAll )];
    }

}  // end namespace fb
