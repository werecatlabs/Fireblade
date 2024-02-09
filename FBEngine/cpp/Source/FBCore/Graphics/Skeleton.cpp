#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Skeleton.h>
#include <FBCore/Interface/Graphics/IBone.h>
#include <FBCore/Core/Properties.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, Skeleton, SharedGraphicsObject<ISkeleton> );

    Skeleton::~Skeleton()
    {
    }

    Skeleton::Skeleton()
    {
    }

    SmartPtr<IBone> Skeleton::createBone()
    {
        return nullptr;
    }

    SmartPtr<IBone> Skeleton::createBone( u32 handle )
    {
        return nullptr;
    }

    SmartPtr<IBone> Skeleton::createBone( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IBone> Skeleton::createBone( const String &name, u32 handle )
    {
        return nullptr;
    }
}  // namespace fb::render
