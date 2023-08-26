#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageObjectsArray.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageObjectsArray, StateMessage );

    StateMessageObjectsArray::StateMessageObjectsArray()
    {
    }

    StateMessageObjectsArray::~StateMessageObjectsArray()
    {
    }

    Array<SmartPtr<ISharedObject>> StateMessageObjectsArray::getObjects() const
    {
        return m_objects;
    }

    void StateMessageObjectsArray::setObjects( const Array<SmartPtr<ISharedObject>> &objects )
    {
        m_objects = objects;
    }
}  // end namespace fb
