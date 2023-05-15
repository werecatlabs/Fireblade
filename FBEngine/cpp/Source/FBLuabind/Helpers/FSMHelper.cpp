#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Helpers/FSMHelper.h"

namespace fb
{

    void FSMHelper::setFSMState( IFSM *fsm, lua_Integer state )
    {
        fsm->setState( (u32)state );
    }

    void FSMHelper::setFSMStateChangeNow( IFSM *fsm, lua_Integer state, bool changeNow )
    {
        fsm->setState( (u32)state, changeNow );
    }

    lua_Integer FSMHelper::getFSMState( IFSM *fsm )
    {
        return 0;//(lua_Integer)fsm->getState();
    }

    void FSMHelper::setFSMInitialState( IFSM *fsm, lua_Integer state )
    {
        //fsm->setInitialState( (u32)state );
    }

    void FSMHelper::_updateObject( IFSM * object, lua_Number t, lua_Number dt )
    {
        //object->update( Thread::TASK_ID_APPLICATION_LOGIC, t, dt );
    }

}  // namespace fb
