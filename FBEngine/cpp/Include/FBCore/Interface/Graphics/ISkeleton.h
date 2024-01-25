#ifndef ISkeleton_h__
#define ISkeleton_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        /** Interface for a Skeleton. */
        class ISkeleton : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ISkeleton() override = default;

            /** Creates a brand new Bone owned by this Skeleton. */
            virtual SmartPtr<IBone> createBone() = 0;

            /** Creates a brand new Bone owned by this Skeleton. */
            virtual SmartPtr<IBone> createBone( u32 handle ) = 0;

            /** Creates a brand new Bone owned by this Skeleton. */
            virtual SmartPtr<IBone> createBone( const String &name ) = 0;

            /** Creates a brand new Bone owned by this Skeleton. */
            virtual SmartPtr<IBone> createBone( const String &name, u32 handle ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // namespace fb

#endif  // ISkeleton_h__
