#ifndef StateMessageContact2_h__
#define StateMessageContact2_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    class StateMessageContact2 : public StateMessage
    {
    public:
        StateMessageContact2();
        ~StateMessageContact2() override;

        hash32 getContactType() const;
        void setContactType( hash32 val );

        Vector2<real_Num> getPosition() const;
        void setPosition( const Vector2<real_Num> &val );

        Vector2<real_Num> getNormal() const;
        void setNormal( const Vector2<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        hash32 m_contactType;
        Vector2<real_Num> m_position;
        Vector2<real_Num> m_normal;
    };
}  // end namespace fb

#endif  // StateMessageContact2_h__
