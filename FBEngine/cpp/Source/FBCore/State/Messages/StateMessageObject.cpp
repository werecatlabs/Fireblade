#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageObject.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageObject, StateMessage );

    StateMessageObject::StateMessageObject() = default;

    StateMessageObject::~StateMessageObject() = default;

    auto StateMessageObject::getObject() const -> SmartPtr<ISharedObject>
    {
        return m_object;
    }

    void StateMessageObject::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }
}  // end namespace fb
