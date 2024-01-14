#ifndef StatePhysicsPosition_h__
#define StatePhysicsPosition_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Math/Vector2.h"

namespace fb
{
    //---------------------------------------------
    class StatePhysicsPosition2 : public StateMessage
    {
    public:
        StatePhysicsPosition2();
        explicit StatePhysicsPosition2( u32 subjectId );

        StatePhysicsPosition2( u32 subjectId, const Vector2<real_Num> &position );

        explicit StatePhysicsPosition2( const Vector2<real_Num> &position );

        Vector2<real_Num> getPosition() const;
        void setPosition( const Vector2<real_Num> &val );

        u32 getSubjectId() const;
        void setSubjectId( u32 val );

        static const hash_type TYPE;

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector2<real_Num> m_position;
        u32 m_subjectId;
    };

    using StatePhysicsPosition2Ptr = SmartPtr<StatePhysicsPosition2>;
}  // end namespace fb

#endif  // StatePhysicsPosition_h__
