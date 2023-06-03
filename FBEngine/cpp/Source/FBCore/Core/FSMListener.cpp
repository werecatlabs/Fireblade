#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FSMListener.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSMListener, SharedObject<IFSMListener> );

    FSMListener::~FSMListener()
    {
        unload( nullptr );
    }

    void FSMListener::load( SmartPtr<ISharedObject> data )
    {
    }

    void FSMListener::unload( SmartPtr<ISharedObject> data )
    {
        m_fsm = nullptr;
    }

    IFSM::ReturnType FSMListener::handleEvent( [[maybe_unused]] u32 state,
                                               [[maybe_unused]] IFSM::Event eventType )
    {
        return IFSM::ReturnType::Ok;
    }

    SmartPtr<IFSM> FSMListener::getFSM() const
    {
        auto fsm = m_fsm.load();
        return fsm.lock();
    }

    void FSMListener::setFSM( SmartPtr<IFSM> fsm )
    {
        m_fsm = fsm;
    }

}  // end namespace fb
