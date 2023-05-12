#ifndef StateMessageObjectsArray_h__
#define StateMessageObjectsArray_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    class StateMessageObjectsArray : public StateMessage
    {
    public:
        StateMessageObjectsArray();
        ~StateMessageObjectsArray() override;

        Array<SmartPtr<ISharedObject>> getObjects() const;
        void setObjects( const Array<SmartPtr<ISharedObject>> &objects );

        FB_CLASS_REGISTER_DECL;

    protected:
        Array<SmartPtr<ISharedObject>> m_objects;
    };
}  // end namespace fb

#endif  // StateMessageObjectsArray_h__
