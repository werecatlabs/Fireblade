#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FSMListener.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSMListener, IFSMListener );

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

    auto FSMListener::handleEvent( [[maybe_unused]] u32 state, [[maybe_unused]] IFSM::Event eventType )
        -> IFSM::ReturnType
    {
        return IFSM::ReturnType::Ok;
    }

    auto FSMListener::getFSM() const -> SmartPtr<IFSM>
    {
        auto fsm = m_fsm.load();
        return fsm.lock();
    }

    void FSMListener::setFSM( SmartPtr<IFSM> fsm )
    {
        m_fsm = fsm;
    }

}  // end namespace fb
