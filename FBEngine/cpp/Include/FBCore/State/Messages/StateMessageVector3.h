#ifndef StateMessagePosition3_h__
#define StateMessagePosition3_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageVector3 : public StateMessage
    {
    public:
        StateMessageVector3();
        explicit StateMessageVector3( const Vector3F &position );
        ~StateMessageVector3() override;

        Vector3F getValue() const;
        void setValue( const Vector3F &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector3F m_position;
    };
}  // end namespace fb

#endif  // StateMessagePosition3_h__
