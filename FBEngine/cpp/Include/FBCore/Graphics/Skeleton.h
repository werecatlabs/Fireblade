#ifndef Skeleton_h__
#define Skeleton_h__

#include <FBCore/Interface/Graphics/ISkeleton.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {
        class Skeleton : public SharedGraphicsObject<ISkeleton>
        {
        public:
            /**
             * Default constructor.
             */
            Skeleton();

            /**
             * Destructor.
             */
            ~Skeleton() override;

            SmartPtr<IBone> createBone() override;

            SmartPtr<IBone> createBone( u32 handle ) override;

            SmartPtr<IBone> createBone( const String &name ) override;

            SmartPtr<IBone> createBone( const String &name, u32 handle ) override;

            FB_CLASS_REGISTER_DECL;
        };
    } // namespace render
}     // namespace fb

#endif  // Skeleton_h__
