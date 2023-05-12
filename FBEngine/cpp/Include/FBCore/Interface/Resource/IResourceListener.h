#ifndef IResourceListener_h__
#define IResourceListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{

    /**
     * Interface for objects that listen for resource events.
     */
    class IResourceListener : public CSharedObject<ISharedObject>
    {
    public:
        /** Destructor. */
        ~IResourceListener() override = default;

        /**
         * Called when a resource of a given type and file name needs to be reloaded.
         *
         * @param typeName The type of the resource.
         * @param fileName The file name of the resource.
         */
        virtual void reloadResource( const String &typeName, const String &fileName ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IResourceListener_h__
