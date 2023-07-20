#ifndef ICharacterController3_h__
#define ICharacterController3_h__

#include "FBCore/Interface/Physics/IPhysicsBody3.h"

namespace fb
{
    namespace physics
    {

        class ICharacterController3 : public IPhysicsBody3
        {
        public:
            ~ICharacterController3() override = default;

            virtual void setPosition( const Vector3F &position ) = 0;
            virtual Vector3F getPosition() const = 0;

            virtual void setOrientation( const QuaternionF &orientation ) = 0;
            virtual QuaternionF getOrientation() const = 0;

            virtual f32 getMoveSpeed() const = 0;
            virtual void setMoveSpeed( f32 moveSpeed ) = 0;

            virtual bool getJump() const = 0;
            virtual void setJump( bool jump ) = 0;
            virtual bool isGrounded() const = 0;

            virtual void _getObject( void **ppObject ) = 0;

            // temp interface
            virtual void setWalkVector( const Vector3F &vector ) = 0;
            virtual void stop() = 0;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // ICharacterController3_h__
