#ifndef IArchive_h__
#define IArchive_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/FileInfo.h>

namespace fb
{
    /** Interface for an archive class.
     * The IArchive class provides an interface for accessing files within an archive.
     */
    class IArchive : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IArchive() override = default;

        /** Open a stream on a given file.
         * Opens a stream on a file within the archive specified by filePath.
         * @param filePath A string containing the path to the file within the archive.
         * @param input If true, the file is opened for input (reading).
         * @param binary If true, the file is opened in binary mode.
         * @param truncate If true, the file is truncated to zero length upon opening.
         * @param ignorePath If true, the file path is ignored.
         * @return A SmartPtr to the stream object, or null if the file could not be opened.
         */
        virtual SmartPtr<IStream> open( const String &filePath, bool input = true, bool binary = false,
                                        bool truncate = false, bool ignorePath = false ) = 0;

        /** Find out if the named file exists.
         * Checks if a file exists in the archive specified by filePath.
         * @param filePath A string containing the path to the file within the archive.
         * @param ignorePath If true, the file path is ignored.
         * @param ignoreCase If true, the case of the file path is ignored.
         * @return True if the file exists, false otherwise.
         */
        virtual bool exists( const String &filePath, bool ignorePath = false,
                             bool ignoreCase = false ) const = 0;

        /* Returns a object that has functionality to process the list.
        @return The file list object. Can be null.
        */
        virtual SmartPtr<IFileList> getFileList() const = 0;

        /* Gets an array of the file data.
        @return An array of the file info in this archive.
        */
        virtual Array<FileInfo> getFiles() const = 0;

        //! get the archive type
        virtual u8 getType() const = 0;

        /** Reports whether this Archive is read-only, or whether the contents can be updated.
         * @return True if the archive is read-only, false otherwise.
         */
        virtual bool isReadOnly() const = 0;

        //! An optionally used password string
        virtual String getPassword() const = 0;

        //! An optionally used password string
        virtual void setPassword( const String &password ) = 0;

        /** Gets the path of this folder archive.
         * @return A string containing the folder path.
         */
        virtual String getPath() const = 0;

        /** Sets the path of this folder archive.
         * @param path A string containing the folder path.
         */
        virtual void setPath( const String &path ) = 0;

        /** Find file info by its ID.
         * @param id The ID of the file info to find.
         * @param fileInfo A reference to a FileInfo object to hold the file info.
         * @param ignorePath If true, the file path is ignored.
         * @return True if the file info was found, false otherwise.
         */
        virtual bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const = 0;

        /** Find file info by its file path.
         * @param filePath The file path of the file info to find.
         * @param fileInfo A reference to a FileInfo object to hold the file info.
         * @param ignorePath If true, the file path is ignored.
         * @return True if the file info was found, false otherwise.
         */
        virtual bool findFileInfo( const String &filePath, FileInfo &fileInfo,
                                   bool ignorePath = false ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IArchive_h__
