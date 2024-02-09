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
        explicit StateMessageVector3( const Vector3<real_Num> &position );
        ~StateMessageVector3() override;

        Vector3<real_Num> getValue() const;
        void setValue( const Vector3<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector3<real_Num> m_position;
    };
}  // end namespace fb

#endif  // StateMessagePosition3_h__
