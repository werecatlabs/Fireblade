#ifndef RigidbodyListener_h__
#define RigidbodyListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace scene
    {

        class RigidbodyListener : public CSharedObject<IEventListener>
        {
        public:
            RigidbodyListener() = default;
            ~RigidbodyListener() override = default;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            void updateChild( SmartPtr<scene::IActor> actor );
            void handleTransform( const Transform3<real_Num> &t );

            Rigidbody *getOwner() const;
            void setOwner( Rigidbody *val );

            //FB_CLASS_REGISTER_DECL;

        protected:
            Rigidbody *m_owner = nullptr;
        };

    }  // namespace scene
}  // namespace fb

#endif  // RigidbodyListener_h__
