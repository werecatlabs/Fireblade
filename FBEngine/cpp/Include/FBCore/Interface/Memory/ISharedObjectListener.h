#ifndef ISharedObjectListener_h__
#define ISharedObjectListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

namespace fb
{

    /** An interface for a shared object listener. */
    class ISharedObjectListener
    {
    public:
        /** Virtual destructor.
         */
        virtual ~ISharedObjectListener() = default;

        /** Loading state changed event. */
        virtual void loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                          LoadingState newState ) = 0;

        /** Function to destroy shared object listener.
        @param ptr A pointer to the object.
        */
        virtual bool destroy( void *ptr ) = 0;
    };
}  // end namespace fb

#endif  // ISharedObjectListener_h__
