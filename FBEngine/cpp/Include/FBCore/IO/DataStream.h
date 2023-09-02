#ifndef DataStream_h__
#define DataStream_h__

#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    class DataStream : public IStream
    {
    public:
        /// Constructor for creating unnamed streams
        DataStream( u16 accessMode = static_cast<u16>( AccessMode::Read ) );

        /// Constructor for creating named streams
        DataStream( const std::string &name, u16 accessMode = static_cast<u16>( AccessMode::Read ) );

        ~DataStream() override;

        /** @copydoc IStream::getFileName */
        String getFileName( void );

        /** @copydoc IStream::setFileName */
        void setFileName( const String &fileName );

        /// Gets the access mode of the stream
        u16 getAccessMode() const;

        /** Reports whether this stream is readable. */
        bool isReadable() const override;

        /** Reports whether this stream is writeable. */
        bool isWriteable() const override;

        // Streaming operators
        template <typename T>
        DataStream &operator>>( T &val );

        /** @copydoc IStream::read */
        size_t read( void *buf, size_t count ) override = 0;

        /** @copydoc IStream::write */
        size_t write( const void *buf, size_t count ) override;

        /** @copydoc IStream::readLine */
        size_t readLine( char *buf, size_t maxCount, const std::string &delim = "\n" ) override;

        /** @copydoc IStream::getLine */
        String getLine( bool trimAfter = true ) override;

        /** Returns a String containing the entire stream.
        @remarks
            This is a convenience method for text streams only, allowing you to
            retrieve a String object containing all the data in the stream.
        */
        String getAsString( void ) override;

        /** Skip a single line from the stream.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
        @param delim The delimiter(s) to stop at
        @return The number of bytes skipped
        */
        size_t skipLine( const std::string &delim = "\n" ) override;

        /** Returns the total size of the data to be read from the stream,
            or 0 if this is indeterminate for this stream.
        */
        size_t size() const override;

        /** Close the stream; this makes further operations invalid. */
        void close( void ) override = 0;

        /** @copydoc IStream::isOpen */
        bool isOpen() const override;

        /** @copydoc IStream::getFileName */
        String getFileName() const override;

        /** @copydoc IStream::setFreeMemory */
        void setFreeMemory( bool freeMemory ) override;

        FileInfo getFileInfo() const;

        void setFileInfo( const FileInfo &fileInfo );

        FB_CLASS_REGISTER_DECL;

    protected:
        FileInfo m_fileInfo;

        /// The name (e.g. resource name) that can be used to identify the source for this data
        /// (optional)
        String m_name;

        /// Size of the data in the stream (may be 0 if size cannot be determined)
        size_t m_size = 0;

        /// What type of access is allowed (AccessMode)
        u16 m_access = 0;
    };
}  // end namespace fb

#endif  // DataStream_h__
