#ifndef __CMemoryFile_h__
#define __CMemoryFile_h__

#include <FBFileSystem/FBFileSystemPrerequisites.h>
#include <FBFileSystem/DataStream.h>

namespace fb
{

    /** Reads from memory. */
    class CMemoryFile : public DataStream
    {
    public:
        CMemoryFile() = default;
        CMemoryFile( void *memory, long len, const String &fileName, bool deleteMemoryWhenDropped );
        ~CMemoryFile() override;

        bool isOpen() const override;

        void close() override;
        bool eof( void ) const override;

        size_t read( void *buffer, size_t sizeToRead ) override;
        size_t write( const void *buffer, size_t sizeToWrite ) override;

        bool seek( long finalPos ) override;
        size_t size() const override;
        size_t tell() const override;

        const c8 *getCharPtr() const;
        void *getData() const;

        void *getBuffer() const;
        void setBuffer( void *val );

        long getLength() const;
        void setLength( long length );

        bool getFreeMemory() const;
        void setFreeMemory( bool freeMemory ) override;

        size_t skipLine( const String &delim = "\n" ) override;
        void skip( long count ) override;

        FB_CLASS_REGISTER_DECL;

    private:
        u8 *m_buffer = nullptr;
        u8 *m_end = nullptr;
        u8 *m_position = nullptr;
        long m_length = 0;
        bool m_freeMemory = false;
    };
}  // end namespace fb

#endif
