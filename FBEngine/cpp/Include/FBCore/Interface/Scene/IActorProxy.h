#ifndef IActorProxy_h__
#define IActorProxy_h__

#include <FBCore/Interface/Scene/IActor.h>

namespace fb
{
    namespace scene
    {

        /**
         * The IActorProxy class represents a proxy for the IActor interface that adds additional
         * functionality related to the construction of actor objects using the builder pattern.
         */
        class IActorProxy : public IActor
        {
        public:
            /**
             * Destructor for the IActorProxy class.
             */
            ~IActorProxy() override = default;

            /**
             * Returns the IDirector object used to construct the actor represented by this proxy.
             *
             * @return A SmartPtr to the IDirector object.
             */
            virtual SmartPtr<IDirector> getDirector() const = 0;

            /**
             * Sets the IDirector object used to construct the actor represented by this proxy.
             *
             * @param director A SmartPtr to the IDirector object to use.
             */
            virtual void setDirector( SmartPtr<IDirector> director ) = 0;

            /** The implementation. */
            virtual SmartPtr<IActor> getActorImpl() const = 0;

            /** The implementation. */
            virtual void setActorImpl( SmartPtr<IActor> actorImpl ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // IActorProxy_h__
