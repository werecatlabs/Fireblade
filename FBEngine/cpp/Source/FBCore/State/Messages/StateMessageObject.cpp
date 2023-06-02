#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageObject.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageObject, StateMessage );


    StateMessageObject::StateMessageObject()
    {
    }

    StateMessageObject::~StateMessageObject()
    {
    }

    SmartPtr<ISharedObject> StateMessageObject::getObject() const
    {
        return m_object;
    }

    void StateMessageObject::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }
}  // end namespace fb
