#ifndef INetworkStream_h__
#define INetworkStream_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for a network stream. */
    class INetworkStream : public ISharedObject
    {
    public:
        /** Destructor. */
        ~INetworkStream() override = default;

        /** Read data from the stream. */
        virtual size_t read( void *buffer, size_t size ) = 0;

        /** Write data to the stream. */
        virtual size_t write( const void *buffer, size_t size ) = 0;
    };

}  // end namespace fb

#endif  // INetworkStream_h__
