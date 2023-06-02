#ifndef IFolderListingW_h__
#define IFolderListingW_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /**
     * @brief An interface for a folder listing with wide-character (Unicode) strings.
     */
    class IFolderExplorerW : public ISharedObject
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        ~IFolderExplorerW() override = default;

        /**
         * @brief Gets the name of the folder.
         * @return The name of the folder as a wide-character string.
         */
        virtual StringW getFolderName() const = 0;

        /**
         * @brief Sets the name of the folder.
         * @param folderName The name of the folder as a wide-character string.
         */
        virtual void setFolderName( const StringW &folderName ) = 0;

        /**
         * @brief Gets the list of files in the folder.
         * @return A reference to the array of file names as wide-character strings.
         */
        virtual Array<StringW> &getFiles() = 0;

        /**
         * @brief Gets the list of files in the folder.
         * @return A const reference to the array of file names as wide-character strings.
         */
        virtual const Array<StringW> &getFiles() const = 0;

        /**
         * @brief Sets the list of files in the folder.
         * @param files The array of file names as wide-character strings.
         */
        virtual void setFiles( const Array<StringW> &files ) = 0;

        /**
         * @brief Gets the list of sub-folders in the folder.
         * @return An array of pointers to sub-folder listings.
         */
        virtual Array<SmartPtr<IFolderExplorerW>> getSubFolders() const = 0;

        /**
         * @brief Sets the list of sub-folders in the folder.
         * @param subFolders An array of pointers to sub-folder listings.
         */
        virtual void setSubFolders( const Array<SmartPtr<IFolderExplorerW>> &subFolders ) = 0;

        /**
         * @brief Adds a sub-folder to the list of sub-folders in the folder.
         * @param subFolder A pointer to the sub-folder listing to add.
         */
        virtual void addSubFolder( SmartPtr<IFolderExplorerW> subFolder ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IFolderListingW_h__
