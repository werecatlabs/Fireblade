#ifndef IFileListener_h__
#define IFileListener_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** An interface for a file listener. */
    class IFileListener : public ISharedObject
    {
    public:
        static const u32 FILE_LISTENER_ACTION_ADD;       ///< Constant for adding a file.
        static const u32 FILE_LISTENER_ACTION_DELETE;    ///< Constant for deleting a file.
        static const u32 FILE_LISTENER_ACTION_MODIFIED;  ///< Constant for modifying a file.

        /** Virtual destructor. */
        ~IFileListener() override = default;

        /** Handles a file action.
        @param action An unsigned 32-bit integer representing the file action to be handled.
        @param folderName A string object containing the folder name.
        @param fileName A string object containing the file name.
        */
        virtual void fileAction( u32 action, const String &folderName, const String &fileName ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IFileListener_h__
