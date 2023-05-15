#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/FBPhysxStream.h"
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"

namespace fb
{
    namespace physics
    {
        using namespace physx;

        PhysxStream::PhysxStream( SmartPtr<IStream> ds )
        {
            m_dataStream = ds;
        }

        PhysxStream::~PhysxStream()
        {
        }

        PxU8 PhysxStream::readByte() const
        {
            PxU8 byte = 0;
            m_dataStream->read( &byte, sizeof( PxU8 ) );
            return byte;
        }

        PxU16 PhysxStream::readWord() const
        {
            PxU16 word = 0;
            m_dataStream->read( &word, sizeof( PxU16 ) );
            return word;
        }

        PxU32 PhysxStream::readDword() const
        {
            PxU32 dword = 0;
            m_dataStream->read( &dword, sizeof( PxU32 ) );
            return dword;
        }

        float PhysxStream::readFloat() const
        {
            float f = 0;
            m_dataStream->read( &f, sizeof( float ) );
            return f;
        }

        double PhysxStream::readDouble() const
        {
            double d = 0;
            m_dataStream->read( &d, sizeof( double ) );
            return d;
        }

        void PhysxStream::readBuffer( void *buffer, PxU32 size ) const
        {
            m_dataStream->read( buffer, size );
        }

        PxInputStream &PhysxStream::storeByte( PxU8 b )
        {
            m_dataStream->write( &b, sizeof( PxU8 ) );
            return *this;
        }

        PxInputStream &PhysxStream::storeWord( PxU16 w )
        {
            m_dataStream->write( &w, sizeof( PxU16 ) );
            return *this;
        }

        PxInputStream &PhysxStream::storeDword( PxU32 d )
        {
            m_dataStream->write( &d, sizeof( PxU32 ) );
            return *this;
        }

        PxInputStream &PhysxStream::storeFloat( PxReal f )
        {
            m_dataStream->write( &f, sizeof( PxReal ) );
            return *this;
        }

        PxInputStream &PhysxStream::storeDouble( PxF64 f )
        {
            m_dataStream->write( &f, sizeof( PxU64 ) );
            return *this;
        }

        PxInputStream &PhysxStream::storeBuffer( const void *buffer, PxU32 size )
        {
            m_dataStream->write( buffer, size );
            return *this;
        }

        SmartPtr<IStream> PhysxStream::getOgreDataStream()
        {
            return m_dataStream;
        }

        PxU32 PhysxStream::read( void *dest, PxU32 count )
        {
            return static_cast<u32>(m_dataStream->read( dest, count ));
        }
    } // end namespace physics
}     // end namespace fb
