#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/FSMBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/Helpers/FSMContainerHelper.h"
#include "FBLuabind/Helpers/FSMHelper.h"

namespace fb
{

    //SmartPtr<IScriptInvoker> _getInvoker( IFSMScriptListener *fsmListener )
    //{
    //    return fsmListener->getInvoker();
    //}

    void bindFSM( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IFSM, ISharedObject, SmartPtr<ISharedObject>>( "IFSM" )
                        .def( "getFsmManager", &IFSM::getFsmManager )
                        .def( "setFsmManager", &IFSM::setFsmManager )
                        .def( "getStateTime", &IFSM::getStateTime )
                        .def( "setStateTime", &IFSM::setStateTime )
                        .def( "getStateTimeElapsed", &IFSM::getStateTimeElapsed )
                        .def( "setState", &IFSM::setState<int> )
                        .def( "getState", &IFSM::getState<int> )
                        .def( "getPreviousState", &IFSM::getPreviousState )
                        .def( "getCurrentState", &IFSM::getCurrentState )
                        .def( "getNewState", &IFSM::getNewState )
                        .def( "setNewState", &IFSM::setNewState )
                        .def( "isPending", &IFSM::isPending )
                        .def( "addListener", &IFSM::addListener )
                        .def( "removeListener", &IFSM::removeListener )
                        .def( "getStateTicks",
                              ( s32( IFSM::* )( Thread::Task ) const ) & IFSM::getStateTicks )
                        .def( "getStateTicks", ( s32( IFSM::* )( void ) const ) & IFSM::getStateTicks )];

        module( L )[class_<IFSMManager, ISharedObject, SmartPtr<ISharedObject>>( "IFSMManager" )
                        .def( "createFSM", &IFSMManager::createFSM )
                        .def( "destroyFSM", &IFSMManager::destroyFSM )
                        .def( "getStateTime", &IFSMManager::getStateTime )
                        .def( "setStateTime", &IFSMManager::setStateTime )
                        .def( "getStateChangeTime", &IFSMManager::getStateChangeTime )
                        .def( "setStateChangeTime", &IFSMManager::setStateChangeTime )
                        .def( "getPreviousState", &IFSMManager::getPreviousState )
                        .def( "getCurrentState", &IFSMManager::getCurrentState )
                        .def( "getNewState", &IFSMManager::getNewState )
                        .def( "setNewState", &IFSMManager::setNewState )
                        .def( "addListener", &IFSMManager::addListener )
                        .def( "removeListener", &IFSMManager::removeListener )
                        .def( "removeListeners", &IFSMManager::removeListeners )
                        .def( "getListenerPriority", &IFSMManager::getListenerPriority )
                        .def( "setListenerPriority", &IFSMManager::setListenerPriority )
                        .def( "getFlagsPtr", &IFSMManager::getFlagsPtr )
                        .def( "getListeners", &IFSMManager::getListeners )];

        module( L )[class_<IFSMListener, IObject, boost::shared_ptr<IObject>>( "IFSMListener" )
                        .def( "getFSM", &IFSMListener::getFSM )
                        .def( "setFSM", &IFSMListener::setFSM )];
    }

}  // end namespace fb
