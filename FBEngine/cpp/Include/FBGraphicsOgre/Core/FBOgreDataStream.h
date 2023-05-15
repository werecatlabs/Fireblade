#ifndef FBOgreDataStream_h__
#define FBOgreDataStream_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <OgreDataStream.h>

namespace fb
{

    class FBOgreDataStream : public Ogre::DataStream
    {
    public:
        FBOgreDataStream() = default;
        FBOgreDataStream( SmartPtr<IStream> stream );
        ~FBOgreDataStream() override;

        size_t read( void *buf, size_t count ) override;
        Ogre::String getLine( bool trimAfter = true ) override;

        void skip( long count ) override;
        void seek( size_t pos ) override;
        size_t tell( void ) const override;

        bool eof( void ) const override;
        void close( void ) override;

        SmartPtr<IStream> getStream() const;
        void setStream( SmartPtr<IStream> stream );

    protected:
        SmartPtr<IStream> m_stream;
    };

}  // end namespace fb

#endif  // FBOgreDataStream_h__
