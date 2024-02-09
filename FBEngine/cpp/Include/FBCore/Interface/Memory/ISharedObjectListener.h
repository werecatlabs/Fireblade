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

        /** Function to destroy shared object listener.
        @param ptr A pointer to the object.
        @return True if the object was destroyed, false otherwise.
        */
        virtual bool destroy( void *ptr ) = 0;
    };
}  // end namespace fb

#endif  // ISharedObjectListener_h__
