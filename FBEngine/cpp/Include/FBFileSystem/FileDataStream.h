#ifndef FileStreamDataStream_h__
#define FileStreamDataStream_h__

#include <FBFileSystem/FBFileSystemPrerequisites.h>
#include <FBFileSystem/DataStream.h>
#include <istream>

namespace fb
{
    class FileDataStream : public DataStream
    {
    public:
        FileDataStream() = default;

        /** Construct a read-only stream from an STL stream.*/
        FileDataStream( std::ifstream *s, bool freeOnClose = true );

        /** Construct a read-write stream from an STL stream. */
        FileDataStream( std::fstream *s, bool freeOnClose = true );

        /** Construct named read-only stream from an STL stream. */
        FileDataStream( const std::string &name, std::ifstream *s, bool freeOnClose = true );

        /** Construct named read-write stream from an STL stream. */
        FileDataStream( const std::string &name, std::fstream *s, bool freeOnClose = true );

        /** Construct named read-only stream from an STL stream. */
        FileDataStream( const std::string &name, std::ifstream *s, size_t size,
                        bool freeOnClose = true );

        /** Construct named read-write stream from an STL stream. */
        FileDataStream( const std::string &name, std::fstream *s, size_t size, bool freeOnClose = true );

        ~FileDataStream() override;

        /** @copydoc DataStream::read
         */
        size_t read( void *buf, size_t count ) override;

        /** @copydoc DataStream::write
         */
        size_t write( const void *buf, size_t count ) override;

        /** @copydoc DataStream::readLine
         */
        size_t readLine( char *buf, size_t maxCount, const std::string &delim = "\n" ) override;

        /** @copydoc DataStream::skip
         */
        void skip( long count ) override;

        /** @copydoc DataStream::seek
         */
        bool seek( long finalPos ) override;

        /** @copydoc DataStream::tell
         */
        size_t tell( void ) const override;

        /** @copydoc DataStream::eof
         */
        bool eof( void ) const override;

        /** @copydoc DataStream::close
         */
        void close( void ) override;

        /** @copydoc IStream::isOpen */
        bool isOpen() const override;

        /** @copydoc IStream::isValid */
        bool isValid() const override;

        std::istream *getInStream() const;
        void setInStream( std::istream *stream );

        std::ifstream *getFStreamRO() const;
        void setFStreamRO( std::ifstream *stream );

        std::fstream *getFStream() const;
        void setFStream( std::fstream *stream );

        FB_CLASS_REGISTER_DECL;

    protected:
        void determineAccess();

        /// Reference to source stream (read)
        std::istream *mInStream = nullptr;

        /// Reference to source file stream (read-only)
        std::ifstream *mFStreamRO = nullptr;

        /// Reference to source file stream (read-write)
        std::fstream *mFStream = nullptr;

        bool mFreeOnClose = true;
    };
}  // end namespace fb

#endif  // FileStreamDataStream_h__
