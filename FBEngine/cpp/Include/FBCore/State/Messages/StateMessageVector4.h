#ifndef StateMessagePosition4_h__
#define StateMessagePosition4_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector4.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageVector4 : public StateMessage
    {
    public:
        StateMessageVector4();
        explicit StateMessageVector4( const Vector4F &position );
        ~StateMessageVector4() override;

        Vector4F getValue() const;
        void setValue( const Vector4F &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector4F m_position;
    };
}  // end namespace fb

#endif  // StateMessagePosition4_h__
