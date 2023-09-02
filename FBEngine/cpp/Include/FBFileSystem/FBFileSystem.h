#ifndef FBFileSystem_h__
#define FBFileSystem_h__

#include <FBFileSystem/FBFileSystemHeaders.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/System/IPlugin.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class FBFileSystem : public ISharedObject
    {
    public:
        FBFileSystem() = default;
        ~FBFileSystem() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void createFactories();

        SmartPtr<IFileSystem> createFileSystem();
    };
}  // end namespace fb

#endif  // FBFileSystem_h__
