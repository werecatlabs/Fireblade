#ifndef _FB_IStream_h__
#define _FB_IStream_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{

    /** Interface for reading and writing data. */
    class IStream : public ISharedObject
    {
    public:
        /** Enum for specifying the access mode of the stream. */
        enum class AccessMode
        {
            Read = 1, /**< The stream is open for reading. */
            Write = 2 /**< The stream is open for writing. */
        };

        /** Virtual destructor. */
        ~IStream() override = default;

        /** Returns true if the stream is opened. */
        virtual bool isOpen() const = 0;

        /** Closes the stream. Further operations are invalid after this. */
        virtual void close() = 0;

        /** Returns true if the stream has reached the end. */
        virtual bool eof() const = 0;

        /** Reads an amount of bytes from the stream.
         * @param buffer Pointer to buffer where read bytes are written to.
         * @param sizeToRead Amount of bytes to read from the stream.
         * @return The number of bytes read.
         */
        virtual size_t read( void *buffer, size_t sizeToRead ) = 0;

        /** Writes the specified number of bytes to the stream (only applicable to
            streams that are not read-only).
         * @param buf Pointer to a buffer containing the bytes to write.
         * @param count Number of bytes to write.
         * @return The number of bytes written.
         */
        virtual size_t write( const void *buf, size_t count ) = 0;

        /** Gets a single line from the stream.
         * The delimiter character is not included in the data returned, and it is skipped over so
         * the next read will occur after it. The buffer contents will include a terminating character.
         * @note If you use this function, you must open the stream in binary mode, otherwise, it'll produce unexpected results.
         * @param buf Reference to a buffer pointer.
         * @param maxCount The maximum length of data to be read, excluding the terminating character.
         * @param delim The delimiter to stop at.
         * @return The number of bytes read, excluding the terminating character.
         */
        virtual size_t readLine( char *buf, size_t maxCount, const String &delim = "\n" ) = 0;

        /** Returns a String containing the next line of data, optionally
            trimmed for whitespace.
         * This is a convenience method for text streams only, allowing you to
         * retrieve a String object containing the next line of data. The data
         * is read up to the next newline character and the result trimmed if
         * required.
         * @note If you use this function, you must open the stream in binary mode, otherwise, it'll produce unexpected results.
         * @param trimAfter If true, the line is trimmed for whitespace (as in String.trim(true,true)).
         * @return A String containing the next line of data.
         */
        virtual String getLine( bool trimAfter = true ) = 0;

        //! Changes position in the stream.
        /** @param finalPos Destination position in the stream.
         * @param relativeMovement If set to true, the position in the stream is
         * changed relative to the current position. Otherwise, the position is changed
         * from the beginning of the stream.
         * @return True if successful, otherwise false.
         */
        virtual bool seek( long finalPos ) = 0;

        //! Get the size of the stream.
        /** @return Size of the stream in bytes.
         */
        virtual size_t size() const = 0;

        //! Get the current position in the file.
        /** \return Current position in the file in bytes. */
        virtual size_t tell() const = 0;

        //! Get name of file.
        /** \return File name as zero terminated character string. */
        virtual String getFileName() const = 0;

        /** Returns a String containing the entire stream.
        @remarks
            This is a convenience method for text streams only, allowing you to
            retrieve a String object containing all the data in the stream.
        */
        virtual String getAsString( void ) = 0;

        /** Sets whether memory is freed when the object is destroyed. */
        virtual void setFreeMemory( bool freeMemory ) = 0;

        /** Skips a line in the stream. */
        virtual size_t skipLine( const String &delim = "\n" ) = 0;

        /** Skip a defined number of bytes. This can also be a negative value, in which case
        the file pointer rewinds a defined number of bytes. */
        virtual void skip( long count ) = 0;

        /** Gets if the stream is readable. */
        virtual bool isReadable() const = 0;

        /** Gets if the stream is writable. */
        virtual bool isWriteable() const = 0;

        virtual FileInfo getFileInfo() const = 0;

        virtual void setFileInfo( const FileInfo &fileInfo ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // _FB_IStream_h__
