#ifndef IMeshLoader_h__
#define IMeshLoader_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    /// Interface for a mesh loader.
    class IMeshLoader : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IMeshLoader() override = default;

        /** Loads a mesh from the resource data. */
        virtual SmartPtr<scene::IActor> loadActor( SmartPtr<IMeshResource> resource ) = 0;

        /** Loads a mesh from the file path provided. */
        virtual SmartPtr<scene::IActor> loadActor( const String &meshPath ) = 0;

        /** Loads a mesh from the resource data. */
        virtual SmartPtr<IMesh> loadMesh( SmartPtr<IMeshResource> resource ) = 0;

        /** Loads a mesh from the file path provided. */
        virtual SmartPtr<IMesh> loadMesh( const String &meshPath ) = 0;

        virtual Array<render::IMaterial> createMaterials( const String &meshPath ) = 0;

        /** Combine nodes into one mesh. */
        virtual bool getUseSingleMesh() const = 0;

        /** Combine nodes into one mesh. */
        virtual void setUseSingleMesh( bool useSingleMesh ) = 0;

        /** Controls logging. */
        virtual bool getQuietMode() const = 0;

        /** Controls logging. */
        virtual void setQuietMode( bool quietMode ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IMeshLoader_h__
