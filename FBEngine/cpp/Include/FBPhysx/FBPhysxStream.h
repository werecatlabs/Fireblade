#ifndef __FBPhysxStream_h__
#define __FBPhysxStream_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include "common/PxIO.h"
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Interface/IO/IStream.h>

namespace fb
{
    namespace physics
    {
        class PhysxStream : public physx::PxInputStream
        {
        public:
            PhysxStream() = default;
            PhysxStream( SmartPtr<IStream> ds );
            ~PhysxStream() override;

            physx::PxU8 readByte() const;
            physx::PxU16 readWord() const;
            physx::PxU32 readDword() const;
            float readFloat() const;
            double readDouble() const;
            void readBuffer( void *buffer, physx::PxU32 size ) const;

            PxInputStream &storeByte( physx::PxU8 b );
            PxInputStream &storeWord( physx::PxU16 w );
            PxInputStream &storeDword( physx::PxU32 d );
            PxInputStream &storeFloat( physx::PxReal f );
            PxInputStream &storeDouble( physx::PxF64 f );
            PxInputStream &storeBuffer( const void *buffer, physx::PxU32 size );

            SmartPtr<IStream> getOgreDataStream();

            physx::PxU32 read( void *dest, physx::PxU32 count ) override;

        private:
            mutable SmartPtr<IStream> m_dataStream;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // PhysxMemoryStream_h__
