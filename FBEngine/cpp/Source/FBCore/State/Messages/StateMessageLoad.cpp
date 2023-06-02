#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageLoad.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageLoad, StateMessage );
    const hash_type StateMessageLoad::LOAD_HASH = StringUtil::getHash( "load" );
    const hash_type StateMessageLoad::RELOAD_HASH = StringUtil::getHash( "reload" );

    const hash_type StateMessageLoad::LOADED_HASH = StringUtil::getHash( "loaded" );
    const hash_type StateMessageLoad::UNLOADED_HASH = StringUtil::getHash( "unloaded" );

    //---------------------------------------------
    StateMessageLoad::StateMessageLoad()
    {
        setType( LOAD_HASH );
    }

    //---------------------------------------------
    StateMessageLoad::~StateMessageLoad()
    {
    }

    //---------------------------------------------
    SmartPtr<ISharedObject> StateMessageLoad::getObject() const
    {
        return m_object;
    }

    //---------------------------------------------
    void StateMessageLoad::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }
}  // end namespace fb
